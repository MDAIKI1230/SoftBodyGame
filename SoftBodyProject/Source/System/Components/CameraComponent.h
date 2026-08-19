#pragma once

#include "CameraConstants.h"

#include "ViewPort.h"
#include "Color.h"

struct CameraComponent
{
private:
	// カメラ描画距離
	float near{ 0.1f };
	float far{ 1000.0f };

	// FOV(度数法)
	float fov{ 60.0f };

	// 優先度
	int priority{ 0 };

	// 生存フラグ
	bool isActive{ true };

	// 投影方式
	ProjectionType projectionType{ ProjectionType::PERSPECTIVE };

	// 画面の消去方式
	ClearMode clearMode{ ClearMode::SOLID_COLOR };

	// SolidModeの時の色
	Color solidColor{ 255,255,255 };

	// 描画範囲矩形
	ViewPort viewPort;
};
