#pragma once

#include "MDMath.h"

class Camera
{
public:
	Camera(Vector3 _pos, Vector3 _target) :
		position{ _pos },
		target{ _target }
	{
	}

	// 位置取得
	const Vector3& GetPos()const { return position; }
	// ターゲット位置取得
	const Vector3& GetTarget()const { return target; }
private:
	// 位置
	Vector3 position{};
	// 見るオブジェクト
	Vector3 target{};
};