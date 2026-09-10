#pragma once

#include <string>
#include <unordered_map>

#include "RagdollBodyDefinition.h"

/*
	ラグドール生成時の情報の構造体
	マップを使用しているが、アクセスを簡単にするために関数を用意してる
*/
struct RagdollDefinition
{
public:
	// 存在の確認
	bool Contains(const std::string& _boneName) const
	{
		return boneToBody.contains(_boneName);
	}
	// 名前から取得
	RagdollBodyDefinition& operator[](const std::string& _boneName)
	{
		return boneToBody[_boneName];
	}
	// 名前から取得
	const RagdollBodyDefinition& operator[](const std::string& _boneName) const
	{
		return boneToBody.at(_boneName);
	}
private:
	std::unordered_map<std::string, RagdollBodyDefinition> boneToBody;
};