#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "SceneComponentData.h"

struct ConstraintEndPointData
{
public:
	std::uint64_t objectID{ 0 };
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };

	MD_DOCUMENT(
		ConstraintEndPointData, "EndPoint",
		MD_FIELD("ObjectID", objectID),
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight)
	);
};

struct PointConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "PointConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		PointConstraintComponentData, "PointConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};

struct DistanceConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "DistanceConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float distance{ 0.0f };
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		DistanceConstraintComponentData, "DistanceConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("Distance", distance),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};

struct HingeConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "HingeConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float positionCompliance{ 0.0f };
	float positionMaxForce{ (std::numeric_limits<float>::max)() };
	float angularCompliance{ 0.0f };
	float angularMaxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		HingeConstraintComponentData, "HingeConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("PositionCompliance", positionCompliance),
		MD_FIELD("PositionMaxForce", positionMaxForce),
		MD_FIELD("AngularCompliance", angularCompliance),
		MD_FIELD("AngularMaxForce", angularMaxForce)
	);
};

struct AngleLimitPointConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "AngleLimitPointConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float angleMinDegrees{ 0.0f };
	float angleMaxDegrees{ 0.0f };
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		AngleLimitPointConstraintComponentData, "AngleLimitPointConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("AngleMinDeg", angleMinDegrees),
		MD_FIELD("AngleMaxDeg", angleMaxDegrees),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};

struct AngleLimitHingeConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "AngleLimitHingeConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float angleMinDegrees{ 0.0f };
	float angleMaxDegrees{ 0.0f };
	float positionCompliance{ 0.0f };
	float positionMaxForce{ (std::numeric_limits<float>::max)() };
	float angularCompliance{ 0.0f };
	float angularMaxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		AngleLimitHingeConstraintComponentData, "AngleLimitHingeConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("AngleMinDeg", angleMinDegrees),
		MD_FIELD("AngleMaxDeg", angleMaxDegrees),
		MD_FIELD("PositionCompliance", positionCompliance),
		MD_FIELD("PositionMaxForce", positionMaxForce),
		MD_FIELD("AngularCompliance", angularCompliance),
		MD_FIELD("AngularMaxForce", angularMaxForce)
	);
};

struct LimitedBallJointConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "LimitedBallJointConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float swingAngleDegrees{ 0.0f };
	float twistAngleMinDegrees{ 0.0f };
	float twistAngleMaxDegrees{ 0.0f };
	float positionCompliance{ 0.0f };
	float positionMaxForce{ (std::numeric_limits<float>::max)() };
	float angularCompliance{ 0.0f };
	float angularMaxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		LimitedBallJointConstraintComponentData, "LimitedBallJointConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("SwingAngleDeg", swingAngleDegrees),
		MD_FIELD("TwistAngleMinDeg", twistAngleMinDegrees),
		MD_FIELD("TwistAngleMaxDeg", twistAngleMaxDegrees),
		MD_FIELD("PositionCompliance", positionCompliance),
		MD_FIELD("PositionMaxForce", positionMaxForce),
		MD_FIELD("AngularCompliance", angularCompliance),
		MD_FIELD("AngularMaxForce", angularMaxForce)
	);
};

struct PointOnLineConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "PointOnLineConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		PointOnLineConstraintComponentData, "PointOnLineConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};

struct LimitedPointOnLineConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "LimitedPointOnLineConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float distance{ 0.0f };
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		LimitedPointOnLineConstraintComponentData, "LimitedPointOnLineConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("Distance", distance),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};

struct SliderConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "SliderConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		SliderConstraintComponentData, "SliderConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};

struct LimitedSliderConstraintComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "LimitedSliderConstraint"; }
public:
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	std::vector<ConstraintEndPointData> endPoints{};
	float distance{ 0.0f };
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		LimitedSliderConstraintComponentData, "LimitedSliderConstraint",
		MD_FIELD("LocalPosition", localPosition),
		MD_FIELD("LocalRotationDeg", localRotationDegrees),
		MD_FIELD("Weight", weight),
		MD_FIELD("EndPoints", endPoints),
		MD_FIELD("Distance", distance),
		MD_FIELD("Compliance", compliance),
		MD_FIELD("MaxForce", maxForce)
	);
};