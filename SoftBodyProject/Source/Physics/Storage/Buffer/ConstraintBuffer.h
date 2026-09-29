#pragma once

#include "PhysicsBufferBase.h"

#include "Constraint.h"
#include "ConstraintRowBatch.h"

class ConstraintBuffer:public PhysicsBufferBase<Constraint>
{
public:
	// 要素全削除
	void Clear() override
	{
		values.clear();
		batches.clear();
	}
	// バッチ追加
	void AddBatch(const ConstraintRowBatch& _batch)
	{
		batches.push_back(_batch);
	}
	// バッチ取得
	const ConstraintRowBatch& GetBatch(uint32_t _index)
	{
		return batches[_index];
	}
	// バッチ追加
	void AddBatch(uint32_t _batch)
	{
		contactBatches.push_back(_batch);
	}
	// バッチ取得
	uint32_t GetContactBatch(uint32_t _index)
	{
		return contactBatches[_index];
	}
private:
	std::vector<ConstraintRowBatch> batches;
	std::vector<uint32_t> contactBatches;
};
