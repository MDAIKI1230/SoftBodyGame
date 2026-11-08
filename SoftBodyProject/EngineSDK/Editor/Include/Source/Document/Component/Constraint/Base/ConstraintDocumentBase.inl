#pragma once

#include <algorithm>
#include <cmath>

#include "GizmoWidgets.h"
#include "Renderer.h"
#include "SceneDocument.h"
#include "TransformComponentDocument.h"

#include "ConstraintDocumentBase.h"

template<class Derived>
void ConstraintDocumentBase<Derived>::AddEndPoint(
	std::uint64_t _objectID,
	const Vector3& _localPosition,
	const Vector3& _localRotationDegrees,
	float _weight)
{
	endPoints.push_back(EndPointDocument{ _objectID, _localPosition, _localRotationDegrees, _weight });
}

template<class Derived>
bool ConstraintDocumentBase<Derived>::RemoveEndPoint(std::uint64_t _objectID)
{
	const auto it = std::find_if(
		endPoints.begin(), endPoints.end(),
		[_objectID](const EndPointDocument& _endPoint)
		{
			return _endPoint.objectID == _objectID;
		});

	if (it == endPoints.end())
	{
		return false;
	}

	endPoints.erase(it);
	return true;
}

template<class Derived>
void ConstraintDocumentBase<Derived>::DrawEndPointGizmos(PropertyTarget _target, const GizmoContext& _context)
{
	DrawEndPointGizmo(_target, _context, &_context.object->GetTransform(), localPosition, localRotationDegrees);

	for (EndPointDocument& endPoint : endPoints)
	{
		const TransformComponentDocument* transform{ FindEndPointTransform(_context, endPoint) };
		if (!transform)
		{
			continue;
		}

		DrawEndPointGizmo(_target, _context, transform, endPoint.localPosition, endPoint.localRotationDegrees);
	}
}

template<class Derived>
const TransformComponentDocument* ConstraintDocumentBase<Derived>::FindEndPointTransform(const GizmoContext& _context, const EndPointDocument& _endPoint) const
{
	if (!_context.scene)
	{
		return nullptr;
	}

	const ObjectDocument* object{ _context.scene->FindObject(_endPoint.objectID) };
	return object ? &object->GetTransform() : nullptr;
}

template<class Derived>
Vector3 ConstraintDocumentBase<Derived>::GetEndPointWorldPosition(const TransformComponentDocument& _transform, const Vector3& _localPosition)
{
	return _transform.position + _transform.GetRotationMatrix() * _localPosition;
}

template<class Derived>
Matrix4x4 ConstraintDocumentBase<Derived>::GetEndPointWorldRotation(const TransformComponentDocument& _transform, const Vector3& _localRotationDegrees)
{
	const Matrix4x4 localRotation{ Quaternion::Euler(
		MDMath::DegToRad(_localRotationDegrees.x),
		MDMath::DegToRad(_localRotationDegrees.y),
		MDMath::DegToRad(_localRotationDegrees.z)).ToMatrix() };
	return _transform.GetRotationMatrix() * localRotation;
}

template<class Derived>
void ConstraintDocumentBase<Derived>::DrawAngleLimitCone(const Vector3& _position, const Vector3& _direction, float _angle)
{
	const float axisLength{ std::cos(_angle) * DrawConstants::AXIS_LENGTH };
	const float radius{ std::abs(std::sin(_angle) * DrawConstants::AXIS_LENGTH) };
	const Vector3 bottomPosition{ _position + _direction * axisLength };

	if (radius <= MathConstants::EPSILON)
	{
		Renderer::DrawLine(_position, bottomPosition, DrawConstants::CONSTRAINT_COLOR);
	}
	else if (std::abs(axisLength) <= MathConstants::EPSILON)
	{
		Renderer::DrawRing(_position, _direction, radius, DrawConstants::CURVE_DIVISIONS, DrawConstants::CONSTRAINT_COLOR);
	}
	else
	{
		Renderer::DrawCone(_position, bottomPosition, radius, DrawConstants::CURVE_DIVISIONS, DrawConstants::CONSTRAINT_COLOR);
	}
}

template<class Derived>
void ConstraintDocumentBase<Derived>::DrawLineConstraintDebug(const GizmoContext& _context, bool _limited, float _distance, bool _drawRotation) const
{
	const Vector3 ownerPosition{ GetEndPointWorldPosition(_context.object->GetTransform(), localPosition) };
	const Matrix4x4 ownerRotation{ GetEndPointWorldRotation(_context.object->GetTransform(), localRotationDegrees) };
	Vector3 direction{ ownerRotation * Vector3::UP };
	if (direction.LengthSqr() <= MathConstants::EPSILON)
	{
		return;
	}
	direction.Normalize();

	Renderer::DrawSphereMesh(ownerPosition, DrawConstants::POINT_RADIUS, DrawConstants::CONSTRAINT_COLOR);
	const float distance{ std::max(_distance, 0.0f) };
	const float axisLength{ _limited ? distance : DrawConstants::AXIS_LENGTH };
	const Vector3 minPosition{ ownerPosition - direction * axisLength };
	const Vector3 maxPosition{ ownerPosition + direction * axisLength };
	Renderer::DrawLine(minPosition, maxPosition, DrawConstants::CONSTRAINT_COLOR);

	if (_limited)
	{
		Renderer::DrawRing(minPosition, direction, DrawConstants::LIMIT_RING_RADIUS, DrawConstants::CURVE_DIVISIONS, DrawConstants::CONSTRAINT_COLOR);
		Renderer::DrawRing(maxPosition, direction, DrawConstants::LIMIT_RING_RADIUS, DrawConstants::CURVE_DIVISIONS, DrawConstants::CONSTRAINT_COLOR);
	}

	if (_drawRotation)
	{
		const Vector3 reference{ ownerRotation * Vector3::RIGHT };
		Renderer::DrawLine(ownerPosition, ownerPosition + reference * DrawConstants::AXIS_LENGTH, DrawConstants::CONSTRAINT_COLOR);
	}

	for (const EndPointDocument& endPoint : endPoints)
	{
		const TransformComponentDocument* transform{ FindEndPointTransform(_context, endPoint) };
		if (!transform)
		{
			continue;
		}

		const Vector3 position{ GetEndPointWorldPosition(*transform, endPoint.localPosition) };
		float projectedDistance{ Vector3::Dot(position - ownerPosition, direction) };
		if (_limited)
		{
			projectedDistance = std::clamp(projectedDistance, -distance, distance);
		}
		const Vector3 projectedPosition{ ownerPosition + direction * projectedDistance };
		if (!_limited)
		{
			Renderer::DrawLine(ownerPosition, projectedPosition, DrawConstants::CONSTRAINT_COLOR);
		}

		Renderer::DrawSphereMesh(position, DrawConstants::POINT_RADIUS, DrawConstants::CONSTRAINT_COLOR);
		Renderer::DrawSphereMesh(projectedPosition, DrawConstants::PROJECTED_POINT_RADIUS, DrawConstants::CONSTRAINT_COLOR);
		Renderer::DrawLine(position, projectedPosition, DrawConstants::ERROR_COLOR);

		if (_drawRotation)
		{
			const Matrix4x4 rotation{ GetEndPointWorldRotation(*transform, endPoint.localRotationDegrees) };
			const Vector3 pointDirection{ rotation * Vector3::UP };
			const Vector3 reference{ rotation * Vector3::RIGHT };
			Renderer::DrawLine(position - pointDirection * DrawConstants::AXIS_LENGTH, position + pointDirection * DrawConstants::AXIS_LENGTH, DrawConstants::CONSTRAINT_COLOR);
			Renderer::DrawLine(position, position + reference * DrawConstants::AXIS_LENGTH, DrawConstants::CONSTRAINT_COLOR);
		}
	}
}

template<class Derived>
void ConstraintDocumentBase<Derived>::DrawEndPointGizmo(PropertyTarget _target, const GizmoContext& _context, const TransformComponentDocument* _transform, Vector3& _localPosition, Vector3& _localRotationDegrees)
{
	Matrix4x4 objectRotation{ _transform->GetRotationMatrix() };
	// 物理側のEndPointと同じく、ローカル位置にはScaleを掛けない。
	Vector3 worldPosition{ _transform->position + objectRotation * _localPosition };
	Matrix4x4 localRotation{ Quaternion::Euler(
		MDMath::DegToRad(_localRotationDegrees.x),
		MDMath::DegToRad(_localRotationDegrees.y),
		MDMath::DegToRad(_localRotationDegrees.z)).ToMatrix() };
	Matrix4x4 worldRotation{ objectRotation * localRotation };

	GizmoContext context{ _context };

	context.world = MatGenerateFunc::Translate(worldPosition) * worldRotation;
	context.base = _transform->GetMatrix();

	switch (context.operation)
	{
	case ImGuizmo::OPERATION::TRANSLATE:
		_target.fieldName = "LocalPosition";
		GizmoWidgets::DrawArrowGizmo(_target, context, _localPosition);
		break;
	case ImGuizmo::OPERATION::ROTATE:
		_target.fieldName = "LocalRotationDeg";
		GizmoWidgets::DrawRingGizmo(_target, context, _localRotationDegrees);
		break;
	default:
		break;
	}
}