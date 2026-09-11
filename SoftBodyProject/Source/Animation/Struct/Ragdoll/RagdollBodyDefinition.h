#pragma once

#include "CollisionConstants.h"

#include "MDMath.h"

#include "RagdollColliderDefinition.h"

/*
	ラグドール生成時のボーンの定義構造体
	リジッドボディを作る際の情報やコライダーの情報を入れる。
*/
struct RagdollBodyDefinition
{
	// Bone空間から見たPhysics Bodyの配置
	Vector3 bodyLocalPosition;
	Quaternion bodyLocalRotation;

	// Collider
	ColliderType colliderShape{ ColliderType::CAPSULE };
	RagdollColliderDefinition colliderDefinition;

	// 質量
	float mass{ 1.0f };
};