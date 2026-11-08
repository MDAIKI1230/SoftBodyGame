#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "MDJson.h"
#include "SceneComponentData.h"
#include "TransformComponentData.h"

namespace ComponentCreateHelper
{
	using ComponentCreateDataFunc = std::unique_ptr<SceneComponentData>(*)(const MDJson::Value&);

	bool CreateTransformData(const MDJson::Value& _value, TransformComponentData& _output);
	std::unique_ptr<SceneComponentData> CreateRigidBodyData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateCharacterControllerData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateBoxColliderData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateSphereColliderData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateCapsuleColliderData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateRendererData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateFrameRendererData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateCameraData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateLightData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreatePointConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateDistanceConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateHingeConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateAngleLimitPointConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateAngleLimitHingeConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateLimitedBallJointConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreatePointOnLineConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateLimitedPointOnLineConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateSliderConstraintData(const MDJson::Value& _value);
	std::unique_ptr<SceneComponentData> CreateLimitedSliderConstraintData(const MDJson::Value& _value);

	extern const std::unordered_map<std::string, ComponentCreateDataFunc> CreateFuncs;
}