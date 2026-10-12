#pragma once

#include <string>
#include <vector>

#include <document.h>

#include "ActiveRagdollDefinition.h"
#include "SkeletonData.h"

class ActiveRagdollLoaderHelper
{
public:
	// Ragdoll定義のパスとActiveRagdoll調整値を読み込む関数
	static bool LoadDefinitionData(
		const rapidjson::Document& _document,
		const SkeletonData* _skeleton,
		std::string& _ragdollPath,
		std::string& _rotationWeightPathString,
		std::string& _positionWeightPathString,
		ActiveRagdollDefinition& _definition);
private:
	// 各グループの調整値を読み込む関数
	static bool LoadGroundDetection(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadMovement(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadControlState(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadUpright(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadBalance(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadConstraint(const rapidjson::Value& _value, const SkeletonData* _skeleton, ActiveRagdollSetting& _setting);
	static bool LoadJump(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);
	static bool LoadRecovery(const rapidjson::Value& _value, ActiveRagdollSetting& _setting);

	// 指定メンバーからfloat値を取得する関数
	static bool GetFloat(const rapidjson::Value& _value, const char* _name, float& _output);
	// 拘束の強さ読み込み
	static bool GetFloatContainer(const rapidjson::Value& _value, const char* _name, const SkeletonData* _skeleton, std::vector<float>& _output);
	// 全体のデフォルト力決める
	static bool GetDefaultPower(const rapidjson::Value& _value, const SkeletonData* _skeleton, std::vector<float>& _output);
	// 階層以下の力決定
	static bool GetBranchesPower(const rapidjson::Value& _value, const SkeletonData* _skeleton, std::vector<float>& _output);
	// ボーン各個人の力決定
	static bool GetPower(const rapidjson::Value& _value, const SkeletonData* _skeleton, std::vector<float>& _output);
	// パスを取得する関数
	static bool GetString(const rapidjson::Value& _value, const char* _name, std::string& _output);

	// 階層以下すべてにパワーを設定する関数
	static void SetChildrenPower(uint32_t _parentIndex, float _weight, const SkeletonData* _skeleton, std::vector<float>& _output);
};