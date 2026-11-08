#pragma once

#include <cstdint>
#include <vector>

#include "../../Base/ComponentDocument.h"
#include "../Settings/EndPointDocument.h"
#include "ConstraintConstants.h"
#include "PropertyTarget.h"
#include "TransformComponentDocument.h"

template<class Derived>
struct ConstraintDocumentBase : public ComponentDocument<Derived>
{
public:
	using ComponentDocument<Derived>::ComponentDocument;

	// 接続先のObject IDと、そのTransformに対する拘束点を追加する。
	void AddEndPoint(
		std::uint64_t _objectID,
		const Vector3& _localPosition = {},
		const Vector3& _localRotationDegrees = {},
		float _weight = 1.0f);

	// 指定したObject IDに一致する先頭の拘束点を削除する。見つからなければfalse。
	bool RemoveEndPoint(std::uint64_t _objectID);

protected:
	using DrawConstants = ConstraintDebugRenderConstants;

	// 所有側と、登録された接続先すべての操作用ギズモを描画する。
	void DrawEndPointGizmos(PropertyTarget _target, const GizmoContext& _context);
	// 接続先が削除・未設定の場合はnullptrを返す。
	const TransformComponentDocument* FindEndPointTransform(const GizmoContext& _context, const EndPointDocument& _endPoint) const;
	// 物理側と同じくScaleを含めず、拘束点をWorldへ変換する。
	static Vector3 GetEndPointWorldPosition(const TransformComponentDocument& _transform, const Vector3& _localPosition);
	static Matrix4x4 GetEndPointWorldRotation(const TransformComponentDocument& _transform, const Vector3& _localRotationDegrees);
	// 角度制限を表す円錐。退化時には線または円を描く。
	static void DrawAngleLimitCone(const Vector3& _position, const Vector3& _direction, float _angle);
	// 点の線上拘束とSliderで共通の、移動範囲・射影・誤差表示。
	void DrawLineConstraintDebug(const GizmoContext& _context, bool _limited, float _distance, bool _drawRotation) const;

public:
	// 所有ObjectのTransformに対する拘束点の位置・回転・重み。
	Vector3 localPosition{};
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };
	// 接続先Objectごとの拘束点。
	std::vector<EndPointDocument> endPoints;

private:
	// 一つの操作用ギズモを描画する。
	void DrawEndPointGizmo(PropertyTarget _target, const GizmoContext& _context, const TransformComponentDocument* _transform, Vector3& _localPosition, Vector3& _localRotationDegrees);
};

#include "ConstraintDocumentBase.inl"