#pragma once

#include <cstdint>

#include "CollisionConstants.h"

struct CollisionFilter
{
public:

	// 指定した種類のいずれかを持っているか
	bool HasCategory(uint32_t _categoryBits) const;
	// 指定した種類をすべて持っているか(0指定はfalse)
	bool HasAllCategories(uint32_t _categoryBits) const;
	// 有効なグループに所属しているか
	bool HasGroup() const;
	// 指定したグループに所属しているか
	bool CompareGroup(uint32_t _groupID) const;
	// 相手と同じグループに所属しているか
	bool CompareGroup(const CollisionFilter& _other) const;
	// 相手と同じグループの同じ部位か
	bool CompareMember(const CollisionFilter& _other) const;
	// 指定した部位が無視対象になっているか
	// グループ一致の判定は含まない
	bool IsMemberIgnored(uint32_t _memberIndex) const;
	// お互いのFilter設定上、衝突できるか
	bool CanCollide(const CollisionFilter& _other) const;
	// 自身の種類を追加
	void AddCategory(uint32_t _categoryBits);
	// 自身の種類を除外
	void RemoveCategory(uint32_t _categoryBits);
	// 指定した種類との衝突を許可
	void AllowCategory(uint32_t _categoryBits);
	// 指定した種類との衝突を除外
	void IgnoreCategory(uint32_t _categoryBits);
	// 同じグループ内で指定した部位を無視
	// 不正な部位番号なら変更せずfalse
	bool IgnoreMember(uint32_t _memberIndex);
	// 指定した部位の無視設定を解除
	bool AllowMember(uint32_t _memberIndex);
	// 同じグループの全部位を無視
	void IgnoreAllMembers();
	// 全部位の無視設定を解除
	void AllowAllMembers();
	// 新しいグループを発行して所属する
	uint32_t CreateGroup();
	// 相手と同じグループに所属する
	void SetGroup(const CollisionFilter& _source);
	// グループ指定
	void SetGroup(uint32_t _source);
	// グループから抜ける
	void ClearGroup();
	uint32_t GetGroupID() const;

public:
	static constexpr uint32_t INVALID_COLLISION_GROUP{ 0 };
	static constexpr uint32_t MEMBER_COUNT{ 64 };

public:
	// 自信がなんなのか
	uint32_t categoryBits{ CollisionTag::Type::NONE };
	// 衝突したい相手
	uint32_t collideMask{ UINT32_MAX };

	// --- 必要な場合のみ使おう ---

	// 同グループ内で判別用
	uint8_t  memberIndex{ 0 };
	// 同グループ内で無視したい相手
	uint64_t ignoreMembers{ 0 };

private:
	// 同じカテゴリ内でもさらに判別したいときなど細かく決めるとき用
	uint32_t groupID{ INVALID_COLLISION_GROUP };

	// すべてのCollisionFilterで共有
	inline static uint32_t nextGroupID{ 1 };
};