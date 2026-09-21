#pragma once

#include <unordered_map>
#include <string>

#include "ObjectBase.h"

#include "SceneComponentData.h"

class ComponentFactory
{
	using CreateComponentFunc = void(*)(ObjectBase*, SceneComponentData*);
public:
	static void CreateTransformComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateBoxColliderComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateSphereColliderComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateRendererComponent(ObjectBase* _obj, SceneComponentData* _data);
public:
	static std::unordered_map<std::string, CreateComponentFunc> CreateFuncs;
};
