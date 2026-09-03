#pragma once

#include "ColliderID.h"

#include "CollisionConstants.h"

struct ColliderComponent
{
public:
	// 無効なColliderComponentを作成
	ColliderComponent() = default;

	// 派生ColliderComponent生成用
	ColliderComponent(ColliderID _id) :
		id{ _id }
	{
	}

	// 仮想デストラクタ
	virtual ~ColliderComponent() = default;

	// Colliderが現在も有効か
	bool IsValid() const;

	// Colliderが有効ならtrue
	explicit operator bool() const;

	// Colliderの種類取得
	ColliderType GetType() const;

	// Sphereか
	bool IsSphere() const;

	// Boxか
	bool IsBox() const;

	// Capsuleか
	bool IsCapsule() const;

	// 同じColliderか比較
	bool operator==(const ColliderComponent& _other) const;

	// Storage内部用ID取得
	ColliderID GetID() const { return id; }

protected:
	// 派生ColliderComponent内部用
	ColliderID id{};
};
