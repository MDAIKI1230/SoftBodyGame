#pragma once

#include <unordered_set>

#include "AssertMacros.h"
#include "PhysicsComponentAPI.h"

template<class T, class... Args>
T* ComponentFactory::GetOrAddComponent(ObjectBase* _obj, const SceneComponentData* _data, const char* _name, Args&&... _args)
{
	auto entries{ _obj->GetComponents<T>() };
	if (_data->instanceIndex < entries.size())
	{
		return &entries[_data->instanceIndex];
	}
	T* result{ _obj->AddComponent<T>(std::forward<Args>(_args)...) };
	MD_DEBUG_LOG_IF(!result, "[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "component storage rejected creation");
	return result;
}

template<class Component, class Constraint, class Data>
Component* ComponentFactory::PrepareConstraint(ObjectManager* _objects, ObjectBase* _obj, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const Data* _data, const char* _name)
{
	if (_data->endPoints.empty())
	{
		MD_DEBUG_LOG("[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "no connection endpoints");
		return nullptr;
	}
	std::unordered_set<std::uint64_t> targets;
	for (const auto& point : _data->endPoints)
	{
		auto target{ _idMap.find(point.objectID) };
		if (point.objectID == 0 || target == _idMap.end() || !_objects->Get(target->second))
		{
			MD_DEBUG_LOG("[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "endpoint object is missing or unspecified");
			return nullptr;
		}
		if (target->second == _obj->GetID() || !targets.insert(point.objectID).second)
		{
			MD_DEBUG_LOG("[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "self-reference or duplicate endpoint object");
			return nullptr;
		}
	}
	if (!PhysicsComponentAPI::PrepareSceneTransform(_obj->GetID()).IsValid())
	{
		MD_DEBUG_LOG("[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "owner Transform is unavailable");
		return nullptr;
	}
	for (const auto& point : _data->endPoints)
	{
		if (!PhysicsComponentAPI::PrepareSceneTransform(_idMap.at(point.objectID)).IsValid())
		{
			MD_DEBUG_LOG("[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "endpoint Transform is unavailable");
			return nullptr;
		}
	}
	Component* component{ GetOrAddComponent<Component>(_obj, _data, _name) };
	if (!component || !component->GetID().IsValid())
	{
		return nullptr;
	}
	auto owner{ component->GetEndPoint() };
	owner.localPosition = _data->localPosition;
	owner.localRotation = ToRotation(_data->localRotationDegrees);
	owner.weight = _data->weight;
	component->SetEndPoint(owner);
	component->RemoveEndPointOtherAll();
	for (const auto& point : _data->endPoints)
	{
		component->AddEndPoint(_objects->Get(_idMap.at(point.objectID)), point.localPosition, ToRotation(point.localRotationDegrees));
	}
	auto points{ PhysicsComponentAPI::EditOtherEndPoints<Constraint>(component->GetID()) };
	if (points.size() != _data->endPoints.size())
	{
		component->RemoveEndPointOtherAll();
		MD_DEBUG_LOG("[SceneLoader] Object %u / %s: %s\n", _obj->GetID().GetIndex(), _name, "physics backend rejected connection endpoints");
		return nullptr;
	}
	for (std::size_t index{ 0 }; index < points.size(); ++index)
	{
		points[index].weight = _data->endPoints[index].weight;
	}
	return component;
}