#pragma once

#include <document.h>

#include "MDMath.h"

#include "BoneMask.h"
#include "SkeletonData.h"

class BoneMaskLoaderHelper
{
public:
	static bool LoadBoneMask(rapidjson::Document& _document, const SkeletonData* _skeleton, BoneMask& _output);

private:
	// デフォルトウェイトを全体に決める
	static bool LoadDefaultWeight(rapidjson::Document& _document, BoneMask& _output);
	// 階層以下のボーンのウェイト決定
	static bool LoadBranchesWeight(rapidjson::Document& _document, const SkeletonData* _skeleton, BoneMask& _output);
	// ボーン各個人のウェイトを決定
	static bool LoadBoneWeight(rapidjson::Document& _document, const SkeletonData* _skeleton, BoneMask& _output);

	// 階層以下のボーンすべてにウェイトを設定する
	static void SetChildrenWeight(uint32_t _parentIndex, float _weight, const SkeletonData* _skeleton, BoneMask& _output);
};