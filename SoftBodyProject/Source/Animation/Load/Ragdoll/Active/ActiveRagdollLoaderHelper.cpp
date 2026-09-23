#include <utility>

#include "ActiveRagdollLoaderHelper.h"

// Ragdoll定義のパスとActiveRagdoll調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadDefinitionData(
	const rapidjson::Document& _document,
	std::string& _ragdollPath,
	ActiveRagdollSetting& _setting)
{
	// Ragdoll定義JSONへのパスを取得する
	if (!_document.HasMember("RagdollPath") || !_document["RagdollPath"].IsString())
	{
		return false;
	}
	std::string ragdollPath{
		_document["RagdollPath"].GetString(),
		_document["RagdollPath"].GetStringLength()
	};
	if (ragdollPath.empty())
	{
		return false;
	}

	// ActiveRagdoll調整値のRootを取得する
	if (!_document.HasMember("Setting") || !_document["Setting"].IsObject())
	{
		return false;
	}
	const rapidjson::Value& settingValue{ _document["Setting"] };

	ActiveRagdollSetting setting;

	// 各制御の調整値を順番に読み込む
	if (!settingValue.HasMember("GroundDetection") ||
		!LoadGroundDetection(settingValue["GroundDetection"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("Movement") ||
		!LoadMovement(settingValue["Movement"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("ControlState") ||
		!LoadControlState(settingValue["ControlState"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("Upright") ||
		!LoadUpright(settingValue["Upright"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("Balance") ||
		!LoadBalance(settingValue["Balance"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("JointDrive") ||
		!LoadJointDrive(settingValue["JointDrive"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("Jump") ||
		!LoadJump(settingValue["Jump"], setting))
	{
		return false;
	}
	if (!settingValue.HasMember("Recovery") ||
		!LoadRecovery(settingValue["Recovery"], setting))
	{
		return false;
	}

	_ragdollPath = std::move(ragdollPath);
	_setting = setting;
	return true;
}

// 接地判定の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadGroundDetection(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "ProbeStartOffset", _setting.groundProbeStartOffset) &&
		GetFloat(_value, "ProbeDistance", _setting.groundProbeDistance) &&
		GetFloat(_value, "MinGroundDot", _setting.minGroundDot);
}

// 移動制御の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadMovement(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "MaxSpeed", _setting.maxSpeed) &&
		GetFloat(_value, "Acceleration", _setting.moveAcceleration) &&
		GetFloat(_value, "Deceleration", _setting.moveDeceleration) &&
		GetFloat(_value, "AirControlScale", _setting.airControlScale);
}

// 制御状態判定の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadControlState(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "AirborneDelay", _setting.airborneDelay) &&
		GetFloat(_value, "FallenUprightDot", _setting.fallenUprightDot) &&
		GetFloat(_value, "FallDelay", _setting.fallDelay) &&
		GetFloat(_value, "RecoveryUprightDot", _setting.recoveryUprightDot) &&
		GetFloat(_value, "RecoveryDelay", _setting.recoveryDelay);
}

// 直立制御の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadUpright(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "Stiffness", _setting.uprightStiffness) &&
		GetFloat(_value, "Damping", _setting.uprightDamping) &&
		GetFloat(_value, "MaxTorque", _setting.maxUprightTorque) &&
		GetFloat(_value, "AirborneScale", _setting.airborneUprightScale);
}

// バランス制御の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadBalance(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "Stiffness", _setting.balanceStiffness) &&
		GetFloat(_value, "Damping", _setting.balanceDamping) &&
		GetFloat(_value, "MaxForce", _setting.maxBalanceForce) &&
		GetFloat(_value, "DeadZone", _setting.balanceDeadZone);
}

// 関節駆動制御の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadJointDrive(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "Stiffness", _setting.jointDriveStiffness) &&
		GetFloat(_value, "Damping", _setting.jointDriveDamping) &&
		GetFloat(_value, "MaxForce", _setting.maxJointDriveForce);
}

// ジャンプ制御の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadJump(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "Height", _setting.jumpHeight) &&
		GetFloat(_value, "GroundIgnoreTime", _setting.jumpGroundIgnoreTime);
}

// 立ち上がり制御の調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadRecovery(const rapidjson::Value& _value, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloat(_value, "LiftScale", _setting.recoveryLiftScale) &&
		GetFloat(_value, "UprightScale", _setting.recoveryUprightScale) &&
		GetFloat(_value, "TargetPelvisHeight", _setting.targetPelvisHeight) &&
		GetFloat(_value, "HeightStiffness", _setting.pelvisHeightStiffness) &&
		GetFloat(_value, "HeightDamping", _setting.pelvisHeightDamping) &&
		GetFloat(_value, "MaxLiftForce", _setting.maxPelvisLiftForce);
}

// 指定メンバーからfloat値を取得する関数
bool ActiveRagdollLoaderHelper::GetFloat(const rapidjson::Value& _value, const char* _name, float& _output)
{
	if (!_value.HasMember(_name) || !_value[_name].IsNumber())
	{
		return false;
	}
	_output = _value[_name].GetFloat();
	return true;
}
