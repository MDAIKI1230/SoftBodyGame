#include <cstdlib>

#include "CollisionFilter.h"

// 指定した種類のいずれかを持っているか
bool CollisionFilter::HasCategory(uint32_t _categoryBits) const
{
	return (categoryBits & _categoryBits) != 0;
}

// 指定した種類をすべて持っているか(0指定はfalse)
bool CollisionFilter::HasAllCategories(uint32_t _categoryBits) const
{
	return _categoryBits != 0 && (categoryBits & _categoryBits) == _categoryBits;
}

// 有効なグループに所属しているか
bool CollisionFilter::HasGroup() const
{
	return groupID != INVALID_COLLISION_GROUP;
}

// 指定したグループに所属しているか
bool CollisionFilter::CompareGroup(uint32_t _groupID) const
{
	return HasGroup() && groupID == _groupID;
}

// 相手と同じグループに所属しているか
bool CollisionFilter::CompareGroup(const CollisionFilter& _other) const
{
	return CompareGroup(_other.groupID);
}

// 相手と同じグループの同じ部位か
bool CollisionFilter::CompareMember(const CollisionFilter& _other) const
{
	return CompareGroup(_other) && memberIndex < MEMBER_COUNT && memberIndex == _other.memberIndex;
}

// 指定した部位が無視対象になっているか
// グループ一致の判定は含まない
bool CollisionFilter::IsMemberIgnored(uint32_t _memberIndex) const
{
	return _memberIndex < MEMBER_COUNT && (ignoreMembers & (1ull << _memberIndex)) != 0;
}

// お互いのFilter設定上、衝突できるか
bool CollisionFilter::CanCollide(const CollisionFilter& _other) const
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
void CollisionFilter::AddCategory(uint32_t _categoryBits)
{
	categoryBits |= _categoryBits;
}

// 自身の種類を除外
void CollisionFilter::RemoveCategory(uint32_t _categoryBits)
{
	categoryBits &= ~_categoryBits;
}

// 指定した種類との衝突を許可
void CollisionFilter::AllowCategory(uint32_t _categoryBits)
{
	collideMask |= _categoryBits;
}

// 指定した種類との衝突を除外
void CollisionFilter::IgnoreCategory(uint32_t _categoryBits)
{
	collideMask &= ~_categoryBits;
}

// 同じグループ内で指定した部位を無視
// 不正な部位番号なら変更せずfalse
bool CollisionFilter::IgnoreMember(uint32_t _memberIndex)
{
	if (_memberIndex >= MEMBER_COUNT)
	{
		return false;
	}

	ignoreMembers |= 1ull << _memberIndex;
	return true;
}

// 指定した部位の無視設定を解除
bool CollisionFilter::AllowMember(uint32_t _memberIndex)
{
	if (_memberIndex >= MEMBER_COUNT)
	{
		return false;
	}

	ignoreMembers &= ~(1ull << _memberIndex);
	return true;
}

// 同じグループの全部位を無視
void CollisionFilter::IgnoreAllMembers()
{
	ignoreMembers = UINT64_MAX;
}

// 全部位の無視設定を解除
void CollisionFilter::AllowAllMembers()
{
	ignoreMembers = 0;
}

uint32_t CollisionFilter::CreateGroup()
{
	// 番号を使い切った場合、重複したIDを発行しない
	if (nextGroupID == INVALID_COLLISION_GROUP)
	{
		std::abort();
	}

	groupID = nextGroupID++;

	return groupID;
}

void CollisionFilter::SetGroup(const CollisionFilter& _source)
{
	groupID = _source.groupID;
}

void CollisionFilter::SetGroup(uint32_t _groupID)
{
	groupID = _groupID;
}

void CollisionFilter::ClearGroup()
{
	groupID = INVALID_COLLISION_GROUP;
}

uint32_t CollisionFilter::GetGroupID() const
{
	return groupID;
}