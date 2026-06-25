#pragma once

#include "MDMath.h"

class Simplex
{
public:
	// 頂点数
	int GetSize() { return size; }
	// 頂点追加
	void Add(const Vector3& _vec);
	// 頂点削除
	void Erase(int _index);
	// 値直
	Vector3& operator[](size_t _index);
private:
	static constexpr int MAX_SIZE{ 4 };
	// 頂点
	Vector3 vertices[MAX_SIZE];
	// 頂点数
	int size{ 0 };
};
