#pragma once

#include <string>

#include <document.h>

#include "ActiveRagdollSetting.h"

class ActiveRagdollLoaderHelper
{
public:
	// Ragdoll定義のパスとActiveRagdoll調整値を読み込む関数
	static bool LoadDefinitionData(
		const rapidjson::Document& _document,
		std::string& _ragdollPath,
		ActiveRagdollSetting& _setting);
private:
	// 各グループの調整値を読み込む関数
	static bool LoadGroundDetection(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadMovement(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadControlState(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadUpright(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadBalance(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadJointDrive(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadJump(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadRecovery(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);

	// 指定メンバーからfloat値を取得する関数
	static bool GetFloat(const rapidjson::Value& _value, const char* _name, float& _output);
};