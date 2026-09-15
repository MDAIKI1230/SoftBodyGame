#include "RagdollLoaderHelper.h"

// Rootから順に読む関数
bool RagdollLoaderHelper::LoadRagdollDefinition(rapidjson::Document& _document, RagdollDefinition& _output)
{
	// Rootかチェック
	if (!_document.HasMember("Root") || !_document["Root"].IsObject())
	{
		return false;
	}

	return LoadBone(_document["Root"], _output, {});
}
// Bone情報ロード関数
bool RagdollLoaderHelper::LoadBone(const rapidjson::Value& _value, RagdollDefinition& _output, std::string_view _parentBoneName)
{
	// 名前の読み込み
	if (!_value.HasMember("Bone") || !_value["Bone"].IsString())
	{
		return false;
	}

	std::string boneName{ _value["Bone"].GetString(),_value["Bone"].GetStringLength() };

	// Body情報のロード
	RagdollBodyDefinition bodyDefinition;
	if (!LoadBody(_value, bodyDefinition))
	{
		return false;
	}

	_output.boneToBody[boneName] = bodyDefinition;

	// Joint情報のロード
	RagdollJointDefinition jointDefinition;
	if (!_parentBoneName.empty())
	{
		if (!LoadJoint(_value, jointDefinition, _parentBoneName))
		{
			return false;
		}

		_output.boneToJoint[boneName] = jointDefinition;
	}

	// 子Bone情報のロード
	if (!_value.HasMember("Children") || !_value["Children"].IsArray())
	{
		return false;
	}

	for (const auto& child : _value["Children"].GetArray())
	{
		if (!LoadBone(child, _output, boneName))
		{
			return false;
		}
	}

	return true;
}
// Body情報ロード関数
bool RagdollLoaderHelper::LoadBody(const rapidjson::Value& _value, RagdollBodyDefinition& _output)
{
	RagdollBodyDefinition loaded;

	// あるかチェック
	if (!_value.HasMember("Body") || !_value["Body"].IsObject())
	{
		return false;
	}

	const rapidjson::Value& body{ _value["Body"] };

	// 位置を取得
	if (!body.HasMember("LocalPosition") ||!body["LocalPosition"].IsObject())
	{
		return false;
	}
	if (!GetVector3(body["LocalPosition"], loaded.bodyLocalPosition))
	{
		return false;
	}

	// 姿勢を取得
	if (!body.HasMember("LocalRotationDeg") || !body["LocalRotationDeg"].IsObject())
	{
		return false;
	}
	if (!GetRotation(body["LocalRotationDeg"], loaded.bodyLocalRotation))
	{
		return false;
	}

	// 質量取得
	if (!body.HasMember("Mass") || !body["Mass"].IsNumber())
	{
		return false;
	}

	loaded.mass = body["Mass"].GetFloat();

	// コライダー取得
	if (!body.HasMember("Collider") || !body["Collider"].IsObject())
	{
		return false;
	}

	if (!GetCollider(body["Collider"], loaded.colliderShape, loaded.colliderDefinition))
	{
		return false;
	}

	_output = std::move(loaded);

	return true;
}
// Joint情報ロード関数
bool RagdollLoaderHelper::LoadJoint(const rapidjson::Value& _value, RagdollJointDefinition& _output, std::string_view _parentBoneName)
{
	RagdollJointDefinition loaded;

	// あるかチェック
	if (!_value.HasMember("Joint") || !_value["Joint"].IsObject())
	{
		return false;
	}

	const rapidjson::Value& joint{ _value["Joint"] };

	// 位置を取得
	if (!joint.HasMember("LocalPosition") || !joint["LocalPosition"].IsObject())
	{
		return false;
	}
	if (!GetVector3(joint["LocalPosition"], loaded.constraintPositionLocalChild))
	{
		return false;
	}

	// 姿勢を取得
	if (!joint.HasMember("LocalRotationDeg") || !joint["LocalRotationDeg"].IsObject())
	{
		return false;
	}
	if (!GetRotation(joint["LocalRotationDeg"], loaded.constraintRotationLocalChild))
	{
		return false;
	}

	// 拘束取得
	if (!GetConstraint(joint, loaded.type, loaded.angleLimit))
	{
		return false;
	}

	loaded.parentBoneName = _parentBoneName;

	_output = std::move(loaded);
	return true;
}

bool RagdollLoaderHelper::GetVector3(const rapidjson::Value& _value, Vector3& _output)
{
	// 引数にオブジェクトが来ているかチェック
	if (!_value.IsObject())
	{
		return false;
	}

	// Vector3の要素があるかチェック
	if (!_value.HasMember("X") || !_value["X"].IsNumber())
	{
		return false;
	}
	if (!_value.HasMember("Y") || !_value["Y"].IsNumber())
	{
		return false;
	}
	if (!_value.HasMember("Z") || !_value["Z"].IsNumber())
	{
		return false;
	}

	// 値渡して終了
	_output = Vector3{ _value["X"].GetFloat(),_value["Y"].GetFloat(), _value["Z"].GetFloat() };

	return true;
}

// 姿勢取得関数
bool RagdollLoaderHelper::GetRotation(const rapidjson::Value& _value, Quaternion& _output)
{
	// 引数にオブジェクトが来ているかチェック
	if (!_value.IsObject())
	{
		return false;
	}

	// Vector3の要素があるかチェック
	if (!_value.HasMember("X") || !_value["X"].IsNumber())
	{
		return false;
	}
	if (!_value.HasMember("Y") || !_value["Y"].IsNumber())
	{
		return false;
	}
	if (!_value.HasMember("Z") || !_value["Z"].IsNumber())
	{
		return false;
	}

	// 値渡して終了
	_output = Quaternion::Euler(
		MDMath::DegToRad(_value["X"].GetFloat()),
		MDMath::DegToRad(_value["Y"].GetFloat()),
		MDMath::DegToRad(_value["Z"].GetFloat()));

	return true;
}
// コライダー取得関数
bool RagdollLoaderHelper::GetCollider(const rapidjson::Value& _value, ColliderType& _type, RagdollColliderDefinition& _output)
{
	// 引数にオブジェクトが来ているかチェック
	if (!_value.IsObject())
	{
		return false;
	}

	if (!_value.HasMember("Type") || !_value["Type"].IsString())
	{
		return false;
	}

	std::string type{ _value["Type"].GetString(),_value["Type"].GetStringLength() };

	if (type == "Capsule")
	{
		if (!_value.HasMember("Height") || !_value["Height"].IsNumber())
		{
			return false;
		}
		if (!_value.HasMember("Radius") || !_value["Radius"].IsNumber())
		{
			return false;
		}

		_type = ColliderType::CAPSULE;
		_output.capsule.height = _value["Height"].GetFloat();
		_output.capsule.radius = _value["Radius"].GetFloat();

		return true;
	}
	else if (type == "Sphere")
	{
		if (!_value.HasMember("Radius") || !_value["Radius"].IsNumber())
		{
			return false;
		}

		_type = ColliderType::SPHERE;
		_output.sphere.radius = _value["Radius"].GetFloat();

		return true;
	}
	else if (type == "Box")
	{
		if (!_value.HasMember("Size") || !_value["Size"].IsObject())
		{
			return false;
		}

		Vector3 size;
		if (!GetVector3(_value["Size"], size))
		{
			return false;
		}

		_type = ColliderType::BOX;
		_output.box.width = size.x;
		_output.box.height = size.y;
		_output.box.depth = size.z;

		return true;
	}

	return false;
}
// 拘束取得関数
bool RagdollLoaderHelper::GetConstraint(const rapidjson::Value& _value, ConstraintType& _type, RagdollJointAngleLimitDefinition& _output)
{
	// 引数にオブジェクトが来ているかチェック
	if (!_value.IsObject())
	{
		return false;
	}

	if (!_value.HasMember("Type") || !_value["Type"].IsString())
	{
		return false;
	}

	std::string type{ _value["Type"].GetString(),_value["Type"].GetStringLength() };

	if (type == "AngleLimitHinge")
	{
		if (!_value.HasMember("AngleLimit") || !_value["AngleLimit"].IsObject())
		{
			return false;
		}

		const rapidjson::Value& angleLimit{ _value["AngleLimit"] };

		if (!angleLimit.HasMember("MinDeg") || !angleLimit["MinDeg"].IsNumber())
		{
			return false;
		}
		if (!angleLimit.HasMember("MaxDeg") || !angleLimit["MaxDeg"].IsNumber())
		{
			return false;
		}

		_type = ConstraintType::ANGLE_LIMIT_HINGE;
		_output.hinge.minAngleRadians = MDMath::DegToRad(angleLimit["MinDeg"].GetFloat());
		_output.hinge.maxAngleRadians = MDMath::DegToRad(angleLimit["MaxDeg"].GetFloat());

		return true;
	}
	else if (type == "LimitedBallJoint")
	{
		if (!_value.HasMember("AngleLimit") || !_value["AngleLimit"].IsObject())
		{
			return false;
		}

		const rapidjson::Value& angleLimit{ _value["AngleLimit"] };

		if (!angleLimit.HasMember("SwingMaxDeg") || !angleLimit["SwingMaxDeg"].IsNumber())
		{
			return false;
		}
		if (!angleLimit.HasMember("TwistMinDeg") || !angleLimit["TwistMinDeg"].IsNumber())
		{
			return false;
		}
		if (!angleLimit.HasMember("TwistMaxDeg") || !angleLimit["TwistMaxDeg"].IsNumber())
		{
			return false;
		}

		_type = ConstraintType::LIMITED_BALL_JOINT;
		_output.swing.maxSwingAngleRadians = MDMath::DegToRad(angleLimit["SwingMaxDeg"].GetFloat());
		_output.swing.minTwistAngleRadians = MDMath::DegToRad(angleLimit["TwistMinDeg"].GetFloat());
		_output.swing.maxTwistAngleRadians = MDMath::DegToRad(angleLimit["TwistMaxDeg"].GetFloat());

		return true;
	}

	return false;
}