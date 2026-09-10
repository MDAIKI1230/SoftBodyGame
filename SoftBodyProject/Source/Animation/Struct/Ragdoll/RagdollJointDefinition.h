#pragma once

#include <string>

#include "MDMath.h"

/*
	ラグドール生成時のボーンの繋がりの定義構造体
*/
struct RagdollJointDefinition
{
	// 親ボーンの名前
	std::string parentBoneName;
	// 子ボーンの名前
	std::string childBoneName;

	// 
	Matrix4x4 parentJointMatrix;
	Transform childJointFrame;
};