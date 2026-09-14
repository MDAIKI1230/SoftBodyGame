#pragma once

#include "MDMath.h"

#include "PhysicsTransformID.h"

/*
	拘束に使う位置や姿勢の情報構造体
*/
struct EndPointFrame
{
	PhysicsTransformID transformID;

	// 拘束点のローカル位置
	Vector3 localPosition;
	// 拘束点のローカル姿勢
	Quaternion localRotation;
};