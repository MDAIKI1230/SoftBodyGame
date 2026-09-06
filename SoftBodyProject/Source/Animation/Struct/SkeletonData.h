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
    std::vector<std::string> boneNames;
    std::vector<uint32_t> parentIndices;

    std::vector<Vector3> bindLocalPositions;
    std::vector<Quaternion> bindLocalRotations;
    std::vector<Vector3> bindLocalScales;

    // 上の情報から作られるTRS行列
    std::vector<Matrix4x4> bindModelMatrices;

    std::unordered_map<std::string, uint32_t> boneLookup;
};
