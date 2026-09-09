#include "BodySnapshotBuffer.h"

// 取得
const RigidBodySnapshot& BodySnapshotBuffer::Get(BodyID _bodyID)
{
	return rigidBodySnapshotFrames[publishedIndex].slots[_bodyID.GetIndex()];
}

// 記録し始め
RigidBodySnapshotFrame& BodySnapshotBuffer::BeginWrite(size_t _size)
{
	// 現在公開していない側を選ぶ
	writingIndex = 1 - publishedIndex;

	RigidBodySnapshotFrame& frame = rigidBodySnapshotFrames[writingIndex];

	// 並列書き込みが始まる前にサイズを確定
	if (frame.slots.size() < _size)
	{
		frame.slots.resize(_size);
	}

	frame.completedTick = currentTick;

	currentTick++;

	return frame;
}

// 外部に公開
void BodySnapshotBuffer::Publish()
{
	publishedIndex = writingIndex;
}

// 変更
RigidBodySnapshot& BodySnapshotBuffer::Edit(BodyID _bodyID)
{
	return rigidBodySnapshotFrames[writingIndex].slots[_bodyID.GetIndex()];
}
