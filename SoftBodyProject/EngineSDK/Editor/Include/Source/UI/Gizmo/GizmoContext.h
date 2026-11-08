#pragma once

#include <optional>

#include <imgui.h>
#include <ImGuizmo.h>

#include "MDMath.h"

struct SceneDocument;
struct ObjectDocument;

struct GizmoContext
{
	Matrix4x4 view;
	Matrix4x4 projection;
	Matrix4x4 world;
	Matrix4x4 base{ Matrix4x4::IDENTITY };
	// 指定時はGizmoの結果を、この行列で編集対象のローカル座標へ戻す。
	std::optional<Matrix4x4> worldToLocal;
	ImGuizmo::MODE mode{ ImGuizmo::LOCAL };
	ImGuizmo::OPERATION operation{ ImGuizmo::TRANSLATE };
	const ObjectDocument* object{ nullptr };
	SceneDocument* scene{ nullptr };
};