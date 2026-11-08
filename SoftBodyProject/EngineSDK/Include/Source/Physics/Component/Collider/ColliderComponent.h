#pragma once

#include "ColliderID.h"

#include "CollisionConstants.h"

#include "CollisionFilter.h"

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

	// フィルター取得
	const CollisionFilter& GetFilter()  const;
	// フィルター変更
	void SetFilter(const CollisionFilter& _filter);

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

	// 指定した種類のいずれかを持っているか
	bool HasCategory(uint32_t _categoryBits) const;
	// 指定した種類をすべて持っているか
	bool HasAllCategories(uint32_t _categoryBits) const;

	// グループに所属しているか
	bool HasGroup() const;
	// 指定したグループに所属しているか
	bool CompareGroup(uint32_t _groupID) const;
	// 相手と同じグループか
	bool CompareGroup(const ColliderComponent& _other) const;
	// 相手と同じグループか
	bool CompareGroup(const CollisionFilter& _other) const;
	// 相手と同じグループの同じ部位か
	bool CompareMember(const ColliderComponent& _other) const;
	// 相手と同じグループの同じ部位か
	bool CompareMember(const CollisionFilter& _other) const;

	// 指定した部位を無視する設定になっているか
	bool IsMemberIgnored(uint32_t _memberIndex) const;
	// お互いのFilter設定上、衝突できるか
	bool CanCollide(const ColliderComponent& _other) const;

	// 自身の種類を追加・除外
	void AddCategory(uint32_t _categoryBits);
	void RemoveCategory(uint32_t _categoryBits);

	// 指定した種類との衝突を許可・除外
	void AllowCategory(uint32_t _categoryBits);
	void IgnoreCategory(uint32_t _categoryBits);

	// 同じグループ内の指定部位との衝突を除外・許可
	bool IgnoreMember(uint32_t _memberIndex);
	bool AllowMember(uint32_t _memberIndex);

	// 同じグループ内の全部位との衝突を除外・許可
	void IgnoreAllMembers();
	void AllowAllMembers();
protected:
	// 派生ColliderComponent内部用
	ColliderID id{};
};
