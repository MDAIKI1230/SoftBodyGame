#pragma once

#include "CameraConstants.h"

#include "ViewPort.h"
#include "Color.h"

struct CameraComponent
{
public:
	// カメラ距離最近距離の取得
	float GetNear() { return near; }
	// カメラ距離最近距離の変更
	void SetNear(float _near) { near = _near; }
	// カメラ距離最遠距離の取得
	float GetFar() { return far; }
	// カメラ距離最遠距離の変更
	void SetFar(float _far) { far = _far; }

	// FOVの取得
	float GetFov() { return fov; }
	// FOVの変更
	void SetFov(float _fov) { far = _fov; }

	// 優先度の取得
	int GetPriority() { return priority; }
	// 優先度の変更
	void SetPriority(int _priority) { priority = _priority; }

	// 生存フラグの取得
	bool GetIsActive() { return isActive; }
	// 生存フラグの変更
	void SetPriority(bool _isActive) { isActive = _isActive; }

	// 投影方式の取得
	ProjectionType GetProjectionType() { return projectionType; }
	// 投影法式の変更
	void SetProjectionType(ProjectionType _projectionType) { projectionType = _projectionType; }

	// 画面の消去法式の取得
	ClearMode GetClearMode() { return clearMode; }
	// 画面の消去法式の変更
	void SetClearMode(ClearMode _clearMode) { clearMode = _clearMode; }

	// SolidModeの時の色の取得
	const Color& GetSolidColor() { return solidColor; }
	// SolidModeの時の色の変更
	void SetSolidColor(Color& _solidColor) { solidColor = _solidColor; }

	// 描画範囲矩形の取得
	const ViewPort& GetViewPort() { return viewPort; }
	// 描画範囲矩形の変更
	void SetViewPort(ViewPort& _viewPort) { viewPort = _viewPort; }
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
