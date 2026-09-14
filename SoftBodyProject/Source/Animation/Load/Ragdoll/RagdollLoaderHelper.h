#pragma once

#include <document.h>

#include "MDMath.h"

#include "RagdollDefinition.h"

class RagdollLoaderHelper
{
public:
	// Rootから順に読む関数
	static bool LoadRagdollDefinition(rapidjson::Document& _document, RagdollDefinition& _output);
private:
	// Bone情報ロード関数
	static bool LoadBone(const rapidjson::Value& _value, RagdollDefinition& _output, std::string_view _parentBoneName);
	// Body情報ロード関数
	static bool LoadBody(const rapidjson::Value& _value, RagdollBodyDefinition& _output);
	// Joint情報ロード関数
	static bool LoadJoint(const rapidjson::Value& _value, RagdollJointDefinition& _output, std::string_view _parentBoneName);

	// ベクトル取得関数
	static bool GetVector3(const rapidjson::Value& _value, Vector3& _output);
	// 姿勢取得関数
	static bool GetRotation(const rapidjson::Value& _value, Quaternion& _output);
	// コライダー取得関数
	static bool GetCollider(const rapidjson::Value& _value, ColliderType& _type, RagdollColliderDefinition& _output);
	// 拘束取得関数
	static bool GetConstraint(const rapidjson::Value& _value, ConstraintType& _type, RagdollJointAngleLimitDefinition& _output);
};