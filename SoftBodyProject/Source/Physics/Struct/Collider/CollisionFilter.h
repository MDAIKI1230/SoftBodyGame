#pragma once

#include <cstdint>

struct CollisionFilter
{
	// 自信がなんなのか
	uint32_t categoryBits{ 1 };
	// 衝突したい相手
	uint32_t collideMask{ UINT32_MAX };

	// --- 必要な場合のみ使おう ---

	// 同じカテゴリ内でも判別用
	uint32_t groupID{ INVALID_COLLISION_GROUP };
	// 同グループ内で判別用
	uint8_t  memberIndex{ 0 };
	// 同グループ内で無視したい相手
	uint64_t ignoreMembers{ 0 };

	static constexpr uint32_t INVALID_COLLISION_GROUP{ 0 };
	static constexpr uint32_t MEMBER_COUNT{ 64 };

	// 指定した種類のいずれかを持っているか
	bool HasCategory(uint32_t _categoryBits) const
	{
		return (categoryBits & _categoryBits) != 0;
	}

	// 指定した種類をすべて持っているか(0指定はfalse)
	bool HasAllCategories(uint32_t _categoryBits) const
	{
		return _categoryBits != 0 && (categoryBits & _categoryBits) == _categoryBits;
	}

	// 有効なグループに所属しているか
	bool HasGroup() const
	{
		return groupID != INVALID_COLLISION_GROUP;
	}

	// 指定したグループに所属しているか
	bool CompareGroup(uint32_t _groupID) const
	{
		return HasGroup() && groupID == _groupID;
	}

	// 相手と同じグループに所属しているか
	bool CompareGroup(const CollisionFilter& _other) const
	{
		return CompareGroup(_other.groupID);
	}

	// 相手と同じグループの同じ部位か
	bool CompareMember(const CollisionFilter& _other) const
	{
		return CompareGroup(_other) && memberIndex < MEMBER_COUNT && memberIndex == _other.memberIndex;
	}

	// 指定した部位が無視対象になっているか
	// グループ一致の判定は含まない
	bool IsMemberIgnored(uint32_t _memberIndex) const
	{
		return _memberIndex < MEMBER_COUNT && (ignoreMembers & (1ull << _memberIndex)) != 0;
	}

	// お互いのFilter設定上、衝突できるか
	bool CanCollide(const CollisionFilter& _other) const
	{
		if ((collideMask & _other.categoryBits) == 0 || (_other.collideMask & categoryBits) == 0)
		{
			return false;
		}

		if (!CompareGroup(_other))
		{
			return true;
		}

		// 部位番号は0～63
		if (memberIndex >= MEMBER_COUNT || _other.memberIndex >= MEMBER_COUNT)
		{
			return false;
		}

		return !IsMemberIgnored(_other.memberIndex) && !_other.IsMemberIgnored(memberIndex);
	}

	// 自身の種類を追加
	void AddCategory(uint32_t _categoryBits)
	{
		categoryBits |= _categoryBits;
	}

	// 自身の種類を除外
	void RemoveCategory(uint32_t _categoryBits)
	{
		categoryBits &= ~_categoryBits;
	}

	// 指定した種類との衝突を許可
	void AllowCategory(uint32_t _categoryBits)
	{
		collideMask |= _categoryBits;
	}

	// 指定した種類との衝突を除外
	void IgnoreCategory(uint32_t _categoryBits)
	{
		collideMask &= ~_categoryBits;
	}

	// 同じグループ内で指定した部位を無視
	// 不正な部位番号なら変更せずfalse
	bool IgnoreMember(uint32_t _memberIndex)
	{
		if (_memberIndex >= MEMBER_COUNT)
		{
			return false;
		}

		ignoreMembers |= 1ull << _memberIndex;
		return true;
	}

	// 指定した部位の無視設定を解除
	bool AllowMember(uint32_t _memberIndex)
	{
		if (_memberIndex >= MEMBER_COUNT)
		{
			return false;
		}

		ignoreMembers &= ~(1ull << _memberIndex);
		return true;
	}

	// 同じグループの全部位を無視
	void IgnoreAllMembers()
	{
		ignoreMembers = UINT64_MAX;
	}

	// 全部位の無視設定を解除
	void AllowAllMembers()
	{
		ignoreMembers = 0;
	}
};