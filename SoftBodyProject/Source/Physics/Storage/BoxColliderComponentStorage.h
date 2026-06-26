#pragma once

#include "SparseSetStorageBase.h"
#include "BoxColliderComponent.h"

class BoxColliderComponentStorage:public SparseSetStorageBase<BoxColliderComponent>
{
public:
	// X方向の長さ
	std::vector<float> width;
	// Y方向の長さ
	std::vector<float> height;
	// Z方向の長さ
	std::vector<float> depth;
	// AABBID
	std::vector<int> aabbId;
};
