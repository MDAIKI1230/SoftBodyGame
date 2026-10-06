#pragma once

#include "MDMath.h"

#include "ColliderComponent.h"
#include "ObjectBase.h"

/*
	衝突のイベントでコライダーを持つゲームオブジェクトに渡す情報構造体
	法線と重なり深さを埋める処理入れてないけど使うときが来たら埋めるようにする。
*/
struct CollisionInfo
{
	// 当たった相手のコライダー
	ColliderComponent otherCollider;
	// 当たった自身のコライダー
	ColliderComponent selfCollider;
	// 相手のゲームオブジェクト
	ObjectBase* other{ nullptr };
	// 衝突法線
	Vector3 normal;
	// 重なり深さ
	float depth{ 0.0f };
};