#pragma once

#include <DxLib.h>
#include <array>

#include "MDMath.h"

struct GizmoContext;

class EditorCamera
{
public:
	// 描画前に呼ぶ
	void Begin(int _screen, float _sceneWidth, float _sceneHeight);
	// 移動やズームなどの入力に対しての処理
	void Update();
	// 描画後に呼ぶ
	void End();

	// 現在のカメラ状態からギズモの描画情報を設定する
	void CreateGizmoContext(GizmoContext& _context) const;

	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix(float _aspect) const;

private:
	bool CanAcceptInput() const;
	bool UpdateInputState();
	void UpdateRotation();
	void UpdateMove();
	void UpdatePan();
	void UpdateZoom();

private:
	Vector3 position{ 0.0f, 120.0f, -350.0f };
	float yaw{ 0.0f };
	float pitch{ 0.0f };
	Quaternion rotation{ Quaternion::AngleAxis(pitch, Vector3::RIGHT) };

	float nearClip{ 1.0f };
	float farClip{ 5000.0f };
	float fov{ MathConstants::PI_FLT / 3.0f };

	float lookSpeed{ 0.005f };
	float panSpeed{ 0.5f };
	float moveSpeed{ 200.0f };
	float zoomSpeed{ 25.0f };

	std::array<bool, 3> mouseButtons{};
	int lastInputFrame{ -1 };

	// 保存用変数
	int previousScreen{ 0 };
	DxLib::MATRIX previousView;
	DxLib::MATRIX previousProjection;
	float previousNear{ 0.0f };
	float previousFar{ 0.0f };
	float previousCenterX{ 0.0f };
	float previousCenterY{ 0.0f };
	RECT previousArea{};
};