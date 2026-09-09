#pragma once

#include <vector>

#include "RigidBodySnapshotFrame.h"

/*
	リジッドボディのスナップショットのバッファ
	使う前にIDの生存確認を忘れないこと。
*/
class BodySnapshotBuffer
{
	friend class BodySnapshotBuildSystem;
public:
	// 取得
	const RigidBodySnapshot& Get(BodyID _bodyID);
private:
	// 記録し始め
	RigidBodySnapshotFrame& BeginWrite(size_t _size);
	// 外部に公開
	void Publish();
	// 変更
	RigidBodySnapshot& Edit(BodyID _bodyID);
private:
	// 現在のフレーム
	uint32_t currentTick{ 1 };

	// 公開中のバッファ
	uint32_t publishedIndex{ 0 };
	// 公開中のバッファ
	uint32_t writingIndex{ 0 };
	// ダブルバッファにしちゃう
	RigidBodySnapshotFrame rigidBodySnapshotFrames[2];
};
