#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

#include "MDMath.h"

/*
    スケルトンそのもののデータ
    ・ボーンの階層構造
    ・各ボーンのTRS行列
    が入っている
    縦の方向で一つのBoneの情報となっている。
    つまり、BoneIndexは各要素の添え字となっている。
    | BoneIndex | 0 | 1 | 2 |
    | position  |   |   |   |
    | rotation  |   |   |   |
    | scale     |   |   |   |
*/
struct SkeletonData
{
public:
    void ReSize(size_t _size)
    {
        boneNames.resize(_size);
        parentIndices.resize(_size);

        bindLocalPositions.resize(_size);
        bindLocalRotations.resize(_size);
        bindLocalScales.resize(_size);

        bindLocalMatrices.resize(_size);

        bindModelPositions.resize(_size);
        bindModelRotations.resize(_size);
        bindModelScales.resize(_size);

        bindModelMatrices.resize(_size);
    }
public:
    std::vector<std::string> boneNames;
    std::vector<uint32_t> parentIndices;

    // --- 親ボーンからのローカル ---

    std::vector<Vector3> bindLocalPositions;
    std::vector<Quaternion> bindLocalRotations;
    std::vector<Vector3> bindLocalScales;

    // 上の情報から作られるTRS行列
    std::vector<Matrix4x4> bindLocalMatrices;

    // --- モデルからのローカル ---

    std::vector<Vector3> bindModelPositions;
    std::vector<Quaternion> bindModelRotations;
    std::vector<Vector3> bindModelScales;

    std::vector<Matrix4x4> bindModelMatrices;

    std::unordered_map<std::string, uint32_t> boneLookup;
};
