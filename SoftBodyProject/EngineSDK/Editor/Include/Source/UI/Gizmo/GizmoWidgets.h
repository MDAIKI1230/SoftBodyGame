#pragma once

#include "SingletonMacros.h"

#include "CommandHistory.h"
#include "GizmoContext.h"
#include "PropertyTarget.h"

class GizmoWidgets
{
public:
	// 三次元ベクトル矢印ギズモ(位置など用)
	static void DrawArrowGizmo(const PropertyTarget& _target, const GizmoContext& _context, Vector3& _value)
		SingletonPublicFunction(void, DrawArrowGizmo, (const PropertyTarget& _target, const GizmoContext& _context, Vector3& _value), (_target, _context, _value))
	// 三次元ベクトル面ギズモ(スケールなど用)
	static void DrawFaceGizmo(const PropertyTarget& _target, const GizmoContext& _context, Vector3& _value)
		SingletonPublicFunction(void, DrawFaceGizmo, (const PropertyTarget& _target, const GizmoContext& _context, Vector3& _value), (_target, _context, _value))
	// 回転ギズモ
	static void DrawRingGizmo(const PropertyTarget& _target, const GizmoContext& _context, Vector3& _value)
		SingletonPublicFunction(void, DrawRingGizmo, (const PropertyTarget& _target, const GizmoContext& _context, Vector3& _value), (_target, _context, _value))
	
	// CommandHistoryを入れる
	static void SetCommandHistory(CommandHistory* _history)
		SingletonPublicFunction(void, SetCommandHistory, (CommandHistory* _history), (_history))
private:
	GizmoWidgets() = default;

	static GizmoWidgets& Instance()
	{
		static GizmoWidgets instance;
		return instance;
	}

	// 共通の履歴を残す処理部分
	template<class T, class Draw>
	bool EditProperty(const PropertyTarget& _target, T& _value, Draw&& _draw);
	
	// 拡縮を含まない回転行列を、Quaternion::Eulerと同じY * X * Zの角度へ戻す。
	void SetRotationMatrix(const Matrix4x4& _matrix, Vector3& _rotationDegrees);
private:
	CommandHistory* history;
};

#include "GizmoWidgets.inl"