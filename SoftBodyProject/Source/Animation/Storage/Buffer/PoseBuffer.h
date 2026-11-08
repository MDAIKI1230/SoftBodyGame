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
		modelFromBoneMatrices.resize(_size);
		boneFromModelMatrices.resize(_size);
	}

	size_t Size() const
	{
		return localMatrices.size();
	}
public:
	std::vector<Vector3> localPositions;
	std::vector<Quaternion> localRotations;
	std::vector<Vector3> localScales;

	// それぞれの親ボーンを基準としたローカル姿勢
	std::vector<Matrix4x4> localMatrices;

	// localMatricesを親階層順に累積した、
	// BoneのLocal空間からModel空間へ変換する行列
	std::vector<Matrix4x4> modelFromBoneMatrices;

	// 自身が親の時、子のローカル姿勢を求めるときに使う行列
	std::vector<Matrix4x4> boneFromModelMatrices;
};
