#pragma once

#include "ActiveRagdollConstants.h"

#include "MDMath.h"

/*
	アクティブラグドールの地面との設置状況を保持する用構造体
*/
struct FootGroundInfo
{
	RagdollGroundState state{ RagdollGroundState::AIRBORNE };
	Vector3 point{};
	Vector3 normal{ Vector3::UP };
	float distance{};
};