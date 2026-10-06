#pragma once

#include "ObjectBase.h"

#include "CollisionInfo.h"

class MonoBehaviour : public ObjectBase
{
public:
	// コンストラクタ
	MonoBehaviour(WorldStorage* _world, EntityID _entityID) :
		ObjectBase{ _world,_entityID }
	{
	}

	// --- 更新系 ---

	// 更新処理
	virtual void Update() {};
	// 物理更新処理
	virtual void FixedUpdate() {};

	// --- 衝突系 ---

	// 衝突始め
	virtual void OnCollisionEnter(CollisionInfo _info) {};
	// 衝突中ずっと
	virtual void OnCollision(CollisionInfo _info) {};
	// 衝突終わり
	virtual void OnCollisionExit(CollisionInfo _info) {};

	// 仮想デストラクタ
	virtual ~MonoBehaviour() = default;
};