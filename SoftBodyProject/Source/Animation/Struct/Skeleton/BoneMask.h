#pragma once

#include <cstdint>
#include <vector>

/*
	PoseLayerに対するマスク情報そのポーズレイヤーのポーズに対してどれだけ動くかを決める。
	BoneIndexと重みの一対一構造にする。
*/
struct BoneMask
{
	std::vector<uint32_t> weights;
};