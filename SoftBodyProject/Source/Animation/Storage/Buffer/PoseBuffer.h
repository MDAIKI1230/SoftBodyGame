#pragma once

#include <vector>

#include "MDMath.h"

/*
	アニメーションなどによって作られる姿勢を残すバッファ
	場合によって目標のポーズにしたり、出力されたポーズにしたりされる。
	使う時、SkeletonDataのBoneの順番と同じにして使う。
*/
struct PoseBuffer
{
public:
	void ReSize(size_t _size)
	{
		localPositions.resize(_size);
		localRotations.resize(_size);
		localScales.resize(_size);

		localMatrices.resize(_size);
	}

public:
	std::vector<Vector3> localPositions;
	std::vector<Quaternion> localRotations;
	std::vector<Vector3> localScales;

	std::vector<Matrix4x4> localMatrices;
};
