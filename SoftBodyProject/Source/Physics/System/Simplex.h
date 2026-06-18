#pragma once

#include "MDMath.h"

class Simplex
{
public:
	// 頂点数
	int GetSize() { return size; }
	// 頂点追加
	void Add(const Vector3& _vec);
	// 値直
	Vector3& operator[](size_t _index);
private:
	// 頂点
	Vector3 vertices[4];
	// 頂点数
	int size{ 0 };
};