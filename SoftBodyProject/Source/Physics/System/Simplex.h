#pragma once

#include "MDMath.h"

#include "MinkowskiVertex.h"

class Simplex
{
public:
	// 頂点数
	int GetSize() const { return size; }
	// 頂点追加
	void Add(const MinkowskiVertex& _vec);
	// 頂点削除
	void Erase(int _index);
	// 値直
	MinkowskiVertex& operator[](size_t _index);
private:
	static constexpr int MAX_SIZE{ 4 };
	// 頂点
	MinkowskiVertex vertices[MAX_SIZE];
	// 頂点数
	int size{ 0 };
};
