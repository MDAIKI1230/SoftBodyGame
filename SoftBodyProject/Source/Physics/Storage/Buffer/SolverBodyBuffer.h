#pragma once

#include <unordered_map>

#include "PhysicsBufferBase.h"

#include "SolverBody.h"

class SolverBodyBuffer :public PhysicsBufferBase<SolverBody>
{
public:
	// 対応付け版追加
	uint32_t Add(PhysicsTransformID transformID, const SolverBody& body)
	{
		const uint32_t index{ Emplace(body) };
		bodyMap[transformID] = index;
		return index;
	}
	// メモリ確保
	void Reserve(uint32_t size) override
	{
		values.reserve(size);
		bodyMap.reserve(size);
	}
	// IDからIndex取得
	uint32_t GetIndex(PhysicsTransformID id) const
	{
		return bodyMap.at(id);
	}
	// IDに対応するものがあるか
	bool Has(PhysicsTransformID id) const
	{
		return bodyMap.contains(id);
	}
	// 削除
	void Clear() override
	{
		PhysicsBufferBase::Clear();
		bodyMap.clear();
	}
private:
	// 通常の追加関数をカプセル化
	void Add(const SolverBody& _value) override
	{
	}
private:
	std::unordered_map<PhysicsTransformID, uint32_t> bodyMap;
};
