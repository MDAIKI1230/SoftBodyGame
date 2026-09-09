#include "PhysicsCommandBuffer.h"

// 編集
BodyUpdate& PhysicsCommandBuffer::Edit(BodyID _id)
{
	SecureCapacity(_id.GetIndex());

	/*
		この回すでに変更を呼ばれているかどうかをフレームのスタンプ番号から確認し、
	    変更が呼ばれているのなら対応する変更ログを返し、
		ないのなら、この回の記録の列挙に加える
	*/
	BodyUpdate& update{ bodyUpdates[_id.GetIndex()] };

	// 今回の記録で初めて触るBody
	if (stamps[_id.GetIndex()] != currentStamp)
	{
		stamps[_id.GetIndex()] = currentStamp;

		update = {};
		update.target = _id;

		diaryIndices.push_back(_id.GetIndex());
	}
	// 同じ記録中に同一Indexの別世代へ変わった場合
	else if (update.target != _id)
	{
		update = {};
		update.target = _id;
	}

	return update;
}

// 記録開始
void PhysicsCommandBuffer::BeginRecord()
{
	currentStamp++;
	diaryIndices.clear();
}

// メモリ確保
void PhysicsCommandBuffer::SecureCapacity(uint32_t _index)
{
	// indexに対して欲しいサイズ分増やす
	const uint32_t requiredSize{ _index + 1 };

	if (bodyUpdates.size() >= requiredSize)
	{
		return;
	}

	stamps.resize(requiredSize);
	bodyUpdates.resize(requiredSize);
}

// 変更があったコマンドの全探査用
std::span<const uint32_t> PhysicsCommandBuffer::GetRecordedIndices() const
{
	return diaryIndices;
}

// コマンド入手
const BodyUpdate& PhysicsCommandBuffer::GetBodyUpdate(uint32_t _index) const
{
	return bodyUpdates[_index];
}
