#pragma once

#include "SlotPool.h"

#include "EntityID.h"

template<class ID>
class DataStorageBase
{
public:
	// 生存チェック
	bool IsAlive(ID _id) const
	{
		return slotPool.IsAlive(_id);
	}
	// 実データインデックス取得
	uint32_t GetDenseIndex(ID _id) const
	{
		return slotPool.Get(_id).denseIndex;
	}
	// オーナーエンティティID取得
	EntityID GetOwnerEntity(ID _id) const
	{
		return slotPool.Get(_id).ownerEntity;
	}
protected:
	// ID作成
	ID CreateID(Args&&... _args)
	{
		return slotPool.Create();
	}
	// IDを手放す(SLOTから消える)
	void ReleaseID(ID _id)
	{
		slotPool.Release(_id);
	}
	// SLOT取得
	const SLOT& GetSlot(ID _id) const
	{
		return slotPool.Get(_id);
	}
	// DenseIndexの変更
	uint32_t& EditDenseIndex(ID _id)
	{
		return slotPool.Edit(_id).denseIndex;
	}

	// 汎用アクセサ

	template<auto MEMBER>
	decltype(auto) GetSlotMember(ID _id) const
	{
		return (slotPool.Get(_id).*MEMBER);
	}

	template<auto MEMBER>
	decltype(auto) EditSlotMember(ID _id)
	{
		return (slotPool.Edit(_id).*MEMBER);
	}
private:
	SlotPool<ID> slotPool;
};
