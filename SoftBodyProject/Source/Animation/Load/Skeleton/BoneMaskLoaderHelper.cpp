#include "BoneMaskLoaderHelper.h"

bool BoneMaskLoaderHelper::LoadBoneMask(rapidjson::Document& _document, const SkeletonData* _skeleton, BoneMask& _output)
{
	BoneMask loaded;
	loaded.ReSize(_skeleton->Size());

	if (!LoadDefaultWeight(_document, loaded))
	{
		return false;
	}

	if (!LoadBranchesWeight(_document, _skeleton, loaded))
	{
		return false;
	}

	if (!LoadBoneWeight(_document, _skeleton, loaded))
	{
		return false;
	}

	_output = std::move(loaded);
	return true;
}

// デフォルトウェイトを全体に決める
bool BoneMaskLoaderHelper::LoadDefaultWeight(rapidjson::Document& _document, BoneMask& _output)
{
	// 存在確認
	if (!_document.HasMember("DefaultWeight") || !_document["DefaultWeight"].IsFloat())
	{
		return false;
	}

	float defaultWeight{ _document["DefaultWeight"].GetFloat() };

	for (auto& weight : _output.weights)
	{
		weight = defaultWeight;
	}

	return true;
}

// 階層以下のボーンのウェイト決定
bool BoneMaskLoaderHelper::LoadBranchesWeight(rapidjson::Document& _document, const SkeletonData* _skeleton, BoneMask& _output)
{
	// 存在確認
	if (!_document.HasMember("Branches") || !_document["Branches"].IsArray())
	{
		return false;
	}

	const std::unordered_map<std::string, uint32_t>& boneLookup{ _skeleton->boneLookup };

	// Branches内が空でもOK
	for (auto& branch : _document["Branches"].GetArray())
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

		if (!branch.HasMember("Weight") || !branch["Weight"].IsFloat())
		{
			continue;
		}

		float weight{ branch["Weight"].GetFloat() };

		uint32_t boneIndex{ boneLookup.at(boneName) };

		_output.weights[boneIndex] = weight;

		SetChildrenWeight(boneIndex, weight, _skeleton, _output);
	}

	return true;
}

// ボーン各個人のウェイトを決定
bool BoneMaskLoaderHelper::LoadBoneWeight(rapidjson::Document& _document, const SkeletonData* _skeleton, BoneMask& _output)
{
	// 存在確認
	if (!_document.HasMember("Bones") || !_document["Bones"].IsArray())
	{
		return false;
	}

	const std::unordered_map<std::string, uint32_t>& boneLookup{ _skeleton->boneLookup };

	// Branches内が空でもOK
	for (auto& bone : _document["Bones"].GetArray())
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

		if (!bone.HasMember("Weight") || !bone["Weight"].IsFloat())
		{
			continue;
		}

		float weight{ bone["Weight"].GetFloat() };

		uint32_t boneIndex{ boneLookup.at(boneName) };

		_output.weights[boneIndex] = weight;
	}

	return true;
}

// 階層以下のボーンすべてにウェイトを設定する
void BoneMaskLoaderHelper::SetChildrenWeight(uint32_t _parentIndex, float _weight, const SkeletonData* _skeleton, BoneMask& _output)
{
	for (uint32_t boneIndex{ 0 }; boneIndex < _output.weights.size(); boneIndex++)
	{
		if (_skeleton->parentIndices[boneIndex] != _parentIndex)
		{
			continue;
		}

		_output.weights[boneIndex] = _weight;
		SetChildrenWeight(boneIndex, _weight, _skeleton, _output);
	}
}