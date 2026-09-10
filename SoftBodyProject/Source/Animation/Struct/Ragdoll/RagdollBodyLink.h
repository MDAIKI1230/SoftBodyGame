#pragma once

#include <cstdint>

#include "MDMath.h"

#include "BodyID.h"

/*
    Ragdoll内のボーンの情報
    対応するRigidBodyのIDを持つ
*/
struct RagdollBodyLink
{
    // ボーンのインデックス
    uint32_t boneIndex;
    // ボディのID
    BodyID bodyID;

    // Bone座標から見たRigidBody重心の姿勢
    Vector3 bodyPositionInBoneSpace;
    Quaternion bodyRotationInBoneSpace;

	// 上のオフセットを戻す行列
	Matrix4x4 bodyFromBone;
};
