#include <utility>
#include <unordered_map>

#include "ActiveRagdollLoaderHelper.h"

// Ragdoll定義のパスとActiveRagdoll調整値を読み込む関数
bool ActiveRagdollLoaderHelper::LoadDefinitionData(
	const rapidjson::Document& _document,
	const SkeletonData* _skeleton,
	std::string& _ragdollPath,
	std::string& _rotationWeightPathString,
	std::string& _positionWeightPathString,
	ActiveRagdollDefinition& _definition)
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
	// Mask定義JSONへのパスを取得する(なくてもよい)
	std::string maskPath;
	if (GetString(_document, "BoneMaskPath", maskPath))
	{
		_rotationWeightPathString = maskPath;
		_positionWeightPathString = maskPath;
	}
	else
	{
		GetString(_document, "RotationWeightPathString", _rotationWeightPathString);
		GetString(_document, "PositionWeightPathString", _positionWeightPathString);
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
	if (!settingValue.HasMember("Constraint") ||
		!LoadConstraint(settingValue["Constraint"], _skeleton, setting))
	{
		return false;
	}

	_ragdollPath = std::move(ragdollPath);
	_definition.settings = setting;
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
		GetFloat(_value, "AirControlScale", _setting.airControlScale) &&
		GetFloat(_value, "TurnStiffness", _setting.turnStiffness) &&
		GetFloat(_value, "TurnDamping", _setting.turnDamping) &&
		GetFloat(_value, "MaxTurnTorque", _setting.maxTurnTorque);
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
bool ActiveRagdollLoaderHelper::LoadConstraint(const rapidjson::Value& _value, const SkeletonData* _skeleton, ActiveRagdollSetting& _setting)
{
	if (!_value.IsObject())
	{
		return false;
	}
	return GetFloatContainer(_value, "RotationConstraintCompliances", _skeleton, _setting.rotationConstraintCompliances) &&
		GetFloatContainer(_value, "RoatationConstraintMaxPowers", _skeleton, _setting.roatationConstraintMaxPowers) &&
		GetFloatContainer(_value, "PositionConstraintCompliances", _skeleton, _setting.positionConstraintCompliances) &&
		GetFloatContainer(_value, "PositionConstraintMaxPowers", _skeleton, _setting.positionConstraintMaxPowers);
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

// 拘束の強さ読み込み
bool ActiveRagdollLoaderHelper::GetFloatContainer(const rapidjson::Value& _value, const char* _name, const SkeletonData* _skeleton, std::vector<float>& _output)
{
	if (!_value.HasMember(_name) || !_value[_name].IsObject())
	{
		return false;
	}

	return GetDefaultPower(_value[_name], _skeleton, _output) &&
		GetBranchesPower(_value[_name], _skeleton, _output) &&
		GetPower(_value[_name], _skeleton, _output);
}

// 全体のデフォルト力決める
bool ActiveRagdollLoaderHelper::GetDefaultPower(const rapidjson::Value& _value, const SkeletonData* _skeleton, std::vector<float>& _output)
{
	// 存在確認
	if (!_value.HasMember("DefaultPower") || !_value["DefaultPower"].IsFloat())
	{
		return false;
	}

	_output.resize(_skeleton->Size());

	float defaultWeight{ _value["DefaultPower"].GetFloat() };

	for (auto& weight : _output)
	{
		weight = defaultWeight;
	}

	return true;
}

// 階層以下の力決定
bool ActiveRagdollLoaderHelper::GetBranchesPower(const rapidjson::Value& _value, const SkeletonData* _skeleton, std::vector<float>& _output)
{
	// 存在確認
	if (!_value.HasMember("Branches") || !_value["Branches"].IsArray())
	{
		return false;
	}

	const std::unordered_map<std::string, uint32_t>& boneLookup{ _skeleton->boneLookup };

	// Branches内が空でもOK
	for (auto& branch : _value["Branches"].GetArray())
	{
		if (!branch.HasMember("RootBoneName") || !branch["RootBoneName"].IsString())
		{
			continue;
		}

		std::string boneName{ branch["RootBoneName"].GetString(),branch["RootBoneName"].GetStringLength() };

		// ボーンの名前がないなら失敗
		if (!boneLookup.contains(boneName))
		{
			return false;
		}

		if (!branch.HasMember("Power") || !branch["Power"].IsFloat())
		{
			continue;
		}

		float weight{ branch["Power"].GetFloat() };

		uint32_t boneIndex{ boneLookup.at(boneName) };

		_output[boneIndex] = weight;

		SetChildrenPower(boneIndex, weight, _skeleton, _output);
	}

	return true;
}

// ボーン各個人の力決定
bool ActiveRagdollLoaderHelper::GetPower(const rapidjson::Value& _value, const SkeletonData* _skeleton, std::vector<float>& _output)
{
	// 存在確認
	if (!_value.HasMember("Bones") || !_value["Bones"].IsArray())
	{
		return false;
	}

	const std::unordered_map<std::string, uint32_t>& boneLookup{ _skeleton->boneLookup };

	// Bones内が空でもOK
	for (auto& bone : _value["Bones"].GetArray())
	{
		if (!bone.HasMember("BoneName") || !bone["BoneName"].IsString())
		{
			continue;
		}

		std::string boneName{ bone["BoneName"].GetString(),bone["BoneName"].GetStringLength() };

		// ボーンの名前がないなら失敗
		if (!boneLookup.contains(boneName))
		{
			return false;
		}

		if (!bone.HasMember("Power") || !bone["Power"].IsFloat())
		{
			continue;
		}

		float weight{ bone["Power"].GetFloat() };

		uint32_t boneIndex{ boneLookup.at(boneName) };

		_output[boneIndex] = weight;
	}

	return true;
}

// パスを取得する関数
bool ActiveRagdollLoaderHelper::GetString(const rapidjson::Value& _value, const char* _name, std::string& _output)
{
	if (!_value.HasMember(_name) || !_value[_name].IsString())
	{
		return false;
	}

	std::string path{
		_value[_name].GetString(),
		_value[_name].GetStringLength() };

	if (path.empty())
	{
		return false;
	}

	_output = std::move(path);

	return true;
}

// 階層以下すべてにパワーを設定する関数
void ActiveRagdollLoaderHelper::SetChildrenPower(uint32_t _parentIndex, float _weight, const SkeletonData* _skeleton, std::vector<float>& _output)
{
	for (uint32_t boneIndex{ 0 }; boneIndex < _output.size(); boneIndex++)
	{
		if (_skeleton->parentIndices[boneIndex] != _parentIndex)
		{
			continue;
		}

		_output[boneIndex] = _weight;
		SetChildrenPower(boneIndex, _weight, _skeleton, _output);
	}
}