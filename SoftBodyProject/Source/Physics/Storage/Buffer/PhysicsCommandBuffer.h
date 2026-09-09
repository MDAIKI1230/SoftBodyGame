#pragma once

#include <vector>
#include <span>

#include "BodyUpdate.h"
#include "BodyID.h"

/*
	IDの確認は変更を行う前に確認しておくこと
*/
class PhysicsCommandBuffer
{
public:
	// 編集
	BodyUpdate& Edit(BodyID _id);
	// 記録開始
	void BeginRecord();

	// 変更があったコマンドの全探査用
	std::span<const uint32_t> GetRecordedIndices() const;
	// コマンド入手
	const BodyUpdate& GetBodyUpdate(uint32_t _index) const;
private:
	// メモリ確保
	void SecureCapacity(uint32_t _size);
private:
	std::vector<BodyUpdate> bodyUpdates;

	// 現在のフレームのスタンプ番号
	uint32_t currentStamp{ 1 };

	// 記録があったかどうかの記録
	std::vector<uint32_t> stamps;
	// この回記録があった奴らの列挙
	std::vector<uint32_t> diaryIndices;
};
