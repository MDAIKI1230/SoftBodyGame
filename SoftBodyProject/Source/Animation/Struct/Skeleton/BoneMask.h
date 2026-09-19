#pragma once

#include <cstdint>
#include <vector>

/*
	PoseLayerに対するマスク情報そのポーズレイヤーのポーズに対してどれだけ動くかを決める。
	BoneIndexと重みの一対一構造にする。
*/
struct BoneMask
{
public:
	void ReSize(uint32_t _size)
	{
		weights.resize(_size);
	}
public:
	std::vector<float> weights;
};