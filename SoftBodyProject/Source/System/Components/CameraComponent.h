#pragma once

#include "CameraConstants.h"

#include "ViewPort.h"
#include "Color.h"

struct CameraComponent
{
public:
	// カメラ距離最近距離の取得
	float GetNear() const noexcept { return nearClip; }
	// カメラ距離最近距離の変更
	void SetNear(float _near) noexcept { nearClip = _near; }
	// カメラ距離最遠距離の取得
	float GetFar() const noexcept { return farClip; }
	// カメラ距離最遠距離の変更
	void SetFar(float _far) noexcept { farClip = _far; }

	// FOVの取得
	float GetFov() const noexcept { return fov; }
	// FOVの変更
	void SetFov(float _fov) noexcept { fov = _fov; }

	// 優先度の取得
	int GetPriority() const noexcept { return priority; }
	// 優先度の変更
	void SetPriority(int _priority) noexcept { priority = _priority; }

	// 生存フラグの取得
	bool GetIsActive() const noexcept { return isActive; }
	// 生存フラグの変更
	void SetPriority(bool _isActive) noexcept { isActive = _isActive; }

	// 投影方式の取得
	ProjectionType GetProjectionType() const noexcept { return projectionType; }
	// 投影法式の変更
	void SetProjectionType(ProjectionType _projectionType) noexcept { projectionType = _projectionType; }

	// 画面の消去法式の取得
	ClearMode GetClearMode() const noexcept { return clearMode; }
	// 画面の消去法式の変更
	void SetClearMode(ClearMode _clearMode) noexcept { clearMode = _clearMode; }

	// SolidModeの時の色の取得
	const Color& GetSolidColor() const noexcept { return solidColor; }
	// SolidModeの時の色の変更
	void SetSolidColor(Color& _solidColor) noexcept { solidColor = _solidColor; }

	// 描画範囲矩形の取得
	const ViewPort& GetViewPort() const noexcept { return viewPort; }
	// 描画範囲矩形の変更
	void SetViewPort(ViewPort& _viewPort) noexcept { viewPort = _viewPort; }
private:
	// カメラ描画距離
	float nearClip{ 0.1f };
	float farClip{ 1000.0f };

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
