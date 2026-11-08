#pragma once

#include "MDMath.h"

#include "MonoBehaviour.h"

class PressToStart :public MonoBehaviour
{
public:
	// コンストラクタ
	PressToStart(WorldStorage* _world, EntityID _entityID);

	// --- 更新系 ---

	// 更新処理
	void Update() override;
private:
	Vector3 initialPos{ 250.0f,300.0f,0.0f };
	float time{ 0.0f };
};
