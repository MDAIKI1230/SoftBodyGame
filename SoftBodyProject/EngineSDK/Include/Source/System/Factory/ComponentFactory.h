#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "ObjectBase.h"
#include "ObjectManager.h"
#include "SceneComponentData.h"
#include "SceneFileData.h"
#include "ModelHandle.h"

class ComponentFactory
{
	using CreateComponentFunc = void(*)(ObjectBase*, SceneComponentData*);
	using ResolveComponentFunc = void(*)(ObjectManager*, ObjectBase*, const SceneFileData*, const std::unordered_map<std::uint64_t, EntityID>&, const SceneComponentData*);
public:
	static void CreateTransformComponent(ObjectBase* _obj, const TransformComponentData& _data);
	static void CreateRigidBodyComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateCharacterControllerComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateBoxColliderComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateSphereColliderComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateCapsuleColliderComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateModelRendererComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateFrameRendererComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateCameraComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void CreateLightComponent(ObjectBase* _obj, SceneComponentData* _data);
	static void ResolvePointConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveDistanceConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveHingeConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveAngleLimitPointConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveAngleLimitHingeConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveLimitedBallJointConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolvePointOnLineConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveLimitedPointOnLineConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveSliderConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);
	static void ResolveLimitedSliderConstraintComponent(ObjectManager* _objects, ObjectBase* _obj, const SceneFileData* _scene, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneComponentData* _data);

	static std::unordered_map<std::string, CreateComponentFunc> CreateFuncs;
	static std::unordered_map<std::string, ResolveComponentFunc> ResolveFuncs;
private:
	template<class T, class... Args>
	static T* GetOrAddComponent(ObjectBase* _obj, const SceneComponentData* _data, const char* _name, Args&&... _args);
	template<class Component, class Constraint, class Data>
	static Component* PrepareConstraint(ObjectManager* _objects, ObjectBase* _obj, const std::unordered_map<std::uint64_t, EntityID>& _idMap, const Data* _data, const char* _name);
	static Quaternion ToRotation(const Vector3& _degrees);
	static ModelHandle LoadModel(const std::string& _path);
};

#include "ComponentFactory.inl"