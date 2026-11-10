#pragma once

#include <cstdint>

#include "SceneComponentData.h"

struct ColliderComponentData : public SceneComponentData
{
	int category{ -1 };
	// シーン読み込み時に衝突設定から求める接触用マスク。
	std::uint32_t contactIgnoreMask{ 0 };
};