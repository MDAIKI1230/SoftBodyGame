#pragma once

#include "ColliderComponent.h"
#include "ObjectBase.h"
#include "ContactInfo.h"

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
	// 相手の衝突情報
	ContactInfo otherContact;
	// 衝突自分の情報
	ContactInfo selfContact;
};