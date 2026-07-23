#pragma once

#include <cstdint>

template<class TAG>
struct GenerationalID
{
	// 中身はuint32_t;
	using Index = uint32_t;
	// 中身はuint32_t;
	using Generation = uint32_t;
public:
	// コンストラクタ
	GenerationalID(Index _index, Generation _generation) :
		index{ _index },
		generation{ _generation }
	{
	}

	// Index取得
	Index GetIndex() const { return index; }
	// Generation取得
	Generation GetGeneration() const { return generation; }

	// 全部の変数比較
	bool operator==(const GenerationalID&) const = default;
private:
	Index index{ 0 };
	Generation generation{ 0 };
};
