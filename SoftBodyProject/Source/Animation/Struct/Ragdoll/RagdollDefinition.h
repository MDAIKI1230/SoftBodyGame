#pragma once

#include <string>
#include <unordered_map>

#include "RagdollBodyDefinition.h"
#include "RagdollJointDefinition.h"

/*
	ラグドール生成時の情報の構造体
	マップを使用しているが、アクセスを簡単にするために関数を用意してる
*/
struct RagdollDefinition
{
	std::unordered_map<std::string, RagdollBodyDefinition> boneToBody;
	std::unordered_map<std::string, RagdollJointDefinition> boneToJoint;
};