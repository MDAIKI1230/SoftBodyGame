#include "AssertMacros.h"

#include "NamericalAnalysis.h"

#include "RigidBodyComponentStorage.h"
#include "BoxColliderComponentStorage.h"

#include "PhysicsComponentAPI.h"

// --- Body系 ---

// 追加可否判定
bool PhysicsComponentAPI::CanAddBody(EntityID _entity)
{
	// もし仮に、すでにTransformIDに紐づいているBodyIDがいるならそれ以上他のモノをつけたくないのでflaseを返す
	PhysicsTransformID transformID{ transformStorage->GetOrCreateTransform(_entity) };
	if (bodyStorage->Has(transformID))
	{
		return false;
	}

	return true;
}

// 破壊
void PhysicsComponentAPI::DestroyBody(BodyID _id)
{
	bodyStorage->Destroy(_id);
}
// --- RigidBody ---

// 作成
BodyID PhysicsComponentAPI::CreateRigidBody(EntityID _entity)
{
	return bodyStorage->CreateRigidBody(_entity, transformStorage->GetOrCreateTransform(_entity));
}

// 位置取得
Vector3 PhysicsComponentAPI::GetRigidBodyPosition(BodyID _id)
{
	PhysicsTransformID transformID{ bodyStorage->GetTransformID(_id) };
	return transformStorage->GetPosition(transformID);
}
// 位置変更
void PhysicsComponentAPI::SetRigidBodyPosition(BodyID _id, const Vector3& _position)
{
	PhysicsTransformID transformID{ bodyStorage->GetTransformID(_id) };
	transformStorage->SetPosition(transformID, _position);
}

// 回転取得
Quaternion PhysicsComponentAPI::GetRigidBodyRoatation(BodyID _id)
{
	PhysicsTransformID transformID{ bodyStorage->GetTransformID(_id) };
	return transformStorage->GetRotation(transformID);
}
// 回転変更
void PhysicsComponentAPI::SetRigidBodyRoatation(BodyID _id, const Quaternion& _rotation)
{
	PhysicsTransformID transformID{ bodyStorage->GetTransformID(_id) };
	transformStorage->SetRotation(transformID, _rotation);
}

// 力加算
void PhysicsComponentAPI::AddForce(BodyID _id, const Vector3& _force)
{
	// 加算
	bodyStorage->EditRigidBodyForce(_id) += _force;
}
// トルク加算
void PhysicsComponentAPI::AddTorque(BodyID _id, const Vector3& _torque)
{
	// 加算
	bodyStorage->EditRigidBodyTorque(_id) += _torque;
}
// 速度取得
const Vector3& PhysicsComponentAPI::GetVelocity(BodyID _id)
{
	return bodyStorage->GetRigidBodyVelocity(_id);
}
// 速度変更
void PhysicsComponentAPI::SetVelocity(BodyID _id, const Vector3& _velocity)
{
	bodyStorage->SetRigidBodyVelocity(_id, _velocity);
}
// 角速度取得
const Vector3& PhysicsComponentAPI::GetAngularVelocity(BodyID _id)
{
	return bodyStorage->GetRigidBodyAngularVelocity(_id);
}
// 角速度変更
void PhysicsComponentAPI::SetAngularVelocity(BodyID _id, const Vector3& _omega)
{
	bodyStorage->SetRigidBodyAngularVelocity(_id, _omega);
}
// isGravity取得
bool PhysicsComponentAPI::GetIsGravity(BodyID _id)
{
	return bodyStorage->GetRigidBodyIsGravity(_id);
}
// isGravity変更
void PhysicsComponentAPI::SetIsGravity(BodyID _id, bool _isGravity)
{
	bodyStorage->SetRigidBodyIsGravity(_id, _isGravity);
}
// 重力加速度取得
const Vector3& PhysicsComponentAPI::GetGravity(BodyID _id)
{
	return bodyStorage->GetRigidBodyGravity(_id);
}
// 重力加速度変更
void PhysicsComponentAPI::SetGravity(BodyID _id, const Vector3& _gravity)
{
	bodyStorage->SetRigidBodyGravity(_id, _gravity);
}
// 質量取得
float PhysicsComponentAPI::GetMass(BodyID _id)
{
	return bodyStorage->GetRigidBodyMass(_id);
}
// 質量変更
void PhysicsComponentAPI::SetMass(BodyID _id, float _mass)
{
	bodyStorage->SetRigidBodyMass(_id, _mass);
}

// 回転制限取得
RigidBodyRotationLock PhysicsComponentAPI::GetRotationLock(BodyID _id)
{
	Vector3 mask{ bodyStorage->GetRigidBodyRotationLock(_id) };

	using Type = std::underlying_type_t<RigidBodyRotationLock>;

	Type bits = 0;
	bits |= static_cast<Type>(1.0f - mask.x) << 0;
	bits |= static_cast<Type>(1.0f - mask.y) << 1;
	bits |= static_cast<Type>(1.0f - mask.z) << 2;

	return static_cast<RigidBodyRotationLock>(bits);
}
// 回転制限変更
void PhysicsComponentAPI::SetRotationLock(BodyID _id, RigidBodyRotationLock _lock)
{
	bodyStorage->SetRigidBodyRotationLock(_id, _lock);
}

// 移動制限取得
RigidBodyPositionLock PhysicsComponentAPI::GetPositionLock(BodyID _id)
{
	Vector3 mask{ bodyStorage->GetRigidBodyPositionLock(_id) };

	using Type = std::underlying_type_t<RigidBodyPositionLock>;

	Type bits = 0;
	bits |= static_cast<Type>(1.0f - mask.x) << 0;
	bits |= static_cast<Type>(1.0f - mask.y) << 1;
	bits |= static_cast<Type>(1.0f - mask.z) << 2;

	return static_cast<RigidBodyPositionLock>(bits);
}
// 移動制限変更
void PhysicsComponentAPI::SetPositionLock(BodyID _id, RigidBodyPositionLock _lock)
{
	bodyStorage->SetRigidBodyPositionLock(_id, _lock);
}

// --- ソフト系 ---

// ロープ作成
BodyID PhysicsComponentAPI::CreateRope(EntityID _entity, const RopeUpdateInfo _info)
{
	return bodyStorage->CreateRope(_entity, transformStorage->GetOrCreateTransform(_entity), _info);
}
// クロス作成
BodyID PhysicsComponentAPI::CreateCloth(EntityID _entity, const ClothUpdateInfo _info)
{
	return bodyStorage->CreateCloth(_entity, transformStorage->GetOrCreateTransform(_entity), _info);
}
// ソフトボディ作成
BodyID PhysicsComponentAPI::CreateSoftBody(EntityID _entity, const SoftBodyUpdateInfo _info)
{
	return bodyStorage->CreateSoftBody(_entity, transformStorage->GetOrCreateTransform(_entity), _info);
}

// --- ロープ系 ---

// 全体の長さ取得
float PhysicsComponentAPI::GetLength(BodyID _id)
{
	return bodyStorage->GetRopeLength(_id);
}
// 全体の長さ変更
void PhysicsComponentAPI::SetLength(BodyID _id, float _length)
{
	bodyStorage->SetRopeLength(_id, _length);
}
// 分割数取得
int PhysicsComponentAPI::GetSegmentCount(BodyID _id)
{
	return bodyStorage->GetRopeSegmentCount(_id);
}
// 分割数変更
void PhysicsComponentAPI::SetSegmentCount(BodyID _id, int _segmentCount)
{
	bodyStorage->SetRopeSegmentCount(_id, _segmentCount);
}

// --- クロス系 ---

// 横幅取得
float PhysicsComponentAPI::GetClothWidth(BodyID _id)
{
	return bodyStorage->GetClothWidth(_id);
}
// 横幅変更
void PhysicsComponentAPI::SetClothWidth(BodyID _id, float _width)
{
	bodyStorage->SetClothWidth(_id, _width);
}
// 縦幅取得
float PhysicsComponentAPI::GetClothHeight(BodyID _id)
{
	return bodyStorage->GetClothHeight(_id);
}
// 縦幅変更
void PhysicsComponentAPI::SetClothHeight(BodyID _id, float _height)
{
	bodyStorage->SetClothHeight(_id, _height);
}
// 横分割取得
int PhysicsComponentAPI::GetRowCount(BodyID _id)
{
	return bodyStorage->GetClothRowCount(_id);
}
// 横分割変更
void PhysicsComponentAPI::SetRowCount(BodyID _id, int _rowCount)
{
	bodyStorage->SetClothRowCount(_id, _rowCount);
}
// 縦分割取得
int PhysicsComponentAPI::GetColumnCount(BodyID _id)
{
	return bodyStorage->GetClothColumnCount(_id);
}
// 縦分割変更
void PhysicsComponentAPI::SetColumnCount(BodyID _id, int _columnCount)
{
	bodyStorage->SetClothColumnCount(_id, _columnCount);
}

// --- ソフトボディ系 ---

// 横幅取得
float PhysicsComponentAPI::GetSoftBodyWidth(BodyID _id)
{
	return bodyStorage->GetSoftBodyWidth(_id);
}
// 横幅変更
void PhysicsComponentAPI::SetSoftBodyWidth(BodyID _id, float _width)
{
	bodyStorage->SetSoftBodyWidth(_id, _width);
}
// 縦幅取得
float PhysicsComponentAPI::GetSoftBodyHeight(BodyID _id)
{
	return bodyStorage->GetSoftBodyHeight(_id);
}
// 縦幅変更
void PhysicsComponentAPI::SetSoftBodyHeight(BodyID _id, float _height)
{
	bodyStorage->SetSoftBodyHeight(_id, _height);
}
// 奥行き取得
float PhysicsComponentAPI::GetSoftBodyDepth(BodyID _id)
{
	return bodyStorage->GetSoftBodyDepth(_id);
}
// 奥行き変更
void PhysicsComponentAPI::SetSoftBodyDepth(BodyID _id, float _depth)
{
	bodyStorage->SetSoftBodyDepth(_id, _depth);
}
// 横分割取得
int PhysicsComponentAPI::GetSegmentCountX(BodyID _id)
{
	return bodyStorage->GetSoftBodySegmentCountX(_id);
}
// 横分割変更
void PhysicsComponentAPI::SetSegmentCountX(BodyID _id, int _segmentCountX)
{
	bodyStorage->SetSoftBodySegmentCountX(_id, _segmentCountX);
}
// 縦分割取得
int PhysicsComponentAPI::GetSegmentCountY(BodyID _id)
{
	return bodyStorage->GetSoftBodySegmentCountY(_id);
}
// 縦分割変更
void PhysicsComponentAPI::SetSegmentCountY(BodyID _id, int _segmentCountY)
{
	bodyStorage->SetSoftBodySegmentCountY(_id, _segmentCountY);
}
// 奥行き分割取得
int PhysicsComponentAPI::GetSegmentCountZ(BodyID _id)
{
	return bodyStorage->GetSoftBodySegmentCountZ(_id);
}
// 奥行き分割変更
void PhysicsComponentAPI::SetSegmentCountZ(BodyID _id, int _segmentCountZ)
{
	bodyStorage->SetSoftBodySegmentCountZ(_id, _segmentCountZ);
}

// --- コライダー系 ---

void PhysicsComponentAPI::DestroyCollider(ColliderID _id)
{
	colliderStorage->Destroy(_id);
}

// 球作成
ColliderID PhysicsComponentAPI::CreateSphere(EntityID _entity, float _radius)
{
	return colliderStorage->CreateSphere(_entity, transformStorage->GetOrCreateTransform(_entity), _radius);
}
// Box作成
ColliderID PhysicsComponentAPI::CreateBox(EntityID _entity, const Vector3& _scale)
{
	return colliderStorage->CreateBox(_entity, transformStorage->GetOrCreateTransform(_entity), _scale);
}
// カプセル作成
ColliderID PhysicsComponentAPI::CreateCapsule(EntityID _entity, float _height, float _radius)
{
	return colliderStorage->CreateCapsule(_entity, transformStorage->GetOrCreateTransform(_entity), _height, _radius);
}

// 半径取得
float PhysicsComponentAPI::GetSphereRadius(ColliderID _id)
{
	return colliderStorage->GetSphereColliderRadius(_id);
}
// 半径変更
void PhysicsComponentAPI::SetSphereRadius(ColliderID _id, float _radius)
{
	colliderStorage->SetSphereColliderRadius(_id, _radius);
}

// X方向の長さ取得
float PhysicsComponentAPI::GetBoxWidth(ColliderID _id)
{
	return colliderStorage->GetBoxColliderScale(_id).x;
}
// X方向の長さ変更
void PhysicsComponentAPI::SetBoxWidth(ColliderID _id, float _width)
{
	colliderStorage->EditBoxColliderScale(_id).x = _width;
}
// Y方向の長さ取得
float PhysicsComponentAPI::GetBoxHeight(ColliderID _id)
{
	return colliderStorage->GetBoxColliderScale(_id).y;
}
// Y方向の長さ変更
void PhysicsComponentAPI::SetBoxHeight(ColliderID _id, float _height)
{
	colliderStorage->EditBoxColliderScale(_id).y = _height;
}
// Z方向の長さ取得
float PhysicsComponentAPI::GetBoxDepth(ColliderID _id)
{
	return colliderStorage->GetBoxColliderScale(_id).z;
}
// Z方向の長さ変更
void PhysicsComponentAPI::SetBoxDepth(ColliderID _id, float _depth)
{
	colliderStorage->EditBoxColliderScale(_id).z = _depth;
}

// Y方向の長さ取得
float PhysicsComponentAPI::GetCapsuleHeight(ColliderID _id)
{
	return colliderStorage->GetCapsuleColliderHeight(_id);
}
// Y方向の長さ変更
void PhysicsComponentAPI::SetCapsuleHeight(ColliderID _id, float _height)
{
	colliderStorage->SetCapsuleColliderHeight(_id, _height);
}
// 半径取得
float PhysicsComponentAPI::GetCapsuleRadius(ColliderID _id)
{
	return colliderStorage->GetCapsuleColliderRadius(_id);
}
// 半径変更
void PhysicsComponentAPI::SetCapsuleRadius(ColliderID _id, float _radius)
{
	colliderStorage->SetCapsuleColliderRadius(_id, _radius);
}

// --- 拘束系 ---

// 点拘束作成
ConstraintID PhysicsComponentAPI::CreatePointConstraint(EntityID _entity, const Vector3& _localOffset)
{
	return constraintStorage->CreatePointConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset);
}
// 距離拘束作成
ConstraintID PhysicsComponentAPI::CreateDistanceConstraint(EntityID _entity, const Vector3& _localOffset)
{
	return constraintStorage->CreateDistanceConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset);
}

// 破壊
void PhysicsComponentAPI::DestroyConstraint(ConstraintID _id)
{
	constraintStorage->Destory(_id);
}

// 距離拘束の距離取得
float PhysicsComponentAPI::GetDistance(ConstraintID _id)
{
	return constraintStorage->GetDistanceConstraint(_id).distance;
}
// 距離拘束の距離設定
void PhysicsComponentAPI::SetDistance(ConstraintID _id, float _distance)
{
	constraintStorage->EditDistanceConstraint(_id).distance = _distance;
}

// ヒンジ拘束作成
ConstraintID PhysicsComponentAPI::CreateHingeConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateHingeConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset, _localRotation);
}

// 角度制限付き点拘束作成
ConstraintID PhysicsComponentAPI::CreateAngleLimitPointConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateAngleLimitPointConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset, _localRotation);
}

// 角度制限付きヒンジ拘束作成
ConstraintID PhysicsComponentAPI::CreateAngleLimitHingeConstraint(
	EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateAngleLimitHingeConstraint(
		_entity, transformStorage->GetOrCreateTransform(_entity),
		_localOffset, _localRotation);
}

// SwingTwist拘束作成関数
ConstraintID PhysicsComponentAPI::CreateLimitedBallJointConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateLimitedBallJointConstraint(
		_entity, transformStorage->GetOrCreateTransform(_entity),
		_localOffset, _localRotation
	);
}

// 自信のEndPoint取得
const EndPointFrame& PhysicsComponentAPI::GetEndPoint(ConstraintID _id)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::POINTS:
		return constraintStorage->GetPointConstraint(_id).endPoints[0];
		break;
	case ConstraintType::DISTANCE:
		return constraintStorage->GetDistanceConstraint(_id).endPoints[0];
		break;
	case ConstraintType::HINGE:
		return constraintStorage->GetHingeConstraint(_id).ownerEndPoint;
		break;
	case ConstraintType::ANGLE_LIMIT_POINT:
		return constraintStorage->GetAngleLimitPointConstraint(_id).ownerEndPoint;
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		return constraintStorage->GetAngleLimitHingeConstraint(_id).ownerEndPoint;
		break;
	case ConstraintType::LIMITED_BALL_JOINT:
		return constraintStorage->GetLimitedBallJointConstraint(_id).ownerEndPoint;
		break;
	}

	MD_UNREACHABLE("変な拘束IDを使おうとしてんなぁ");
}
// 自信のEndPoint変更
void PhysicsComponentAPI::SetEndPoint(ConstraintID _id, const EndPointFrame& _endPoint)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::POINTS:
		constraintStorage->EditPointConstraint(_id).endPoints[0] = _endPoint;
		break;
	case ConstraintType::DISTANCE:
		constraintStorage->EditDistanceConstraint(_id).endPoints[0] = _endPoint;
		break;
	case ConstraintType::HINGE:
		constraintStorage->EditHingeConstraint(_id).ownerEndPoint = _endPoint;
		break;
	case ConstraintType::ANGLE_LIMIT_POINT:
		constraintStorage->EditAngleLimitPointConstraint(_id).ownerEndPoint = _endPoint;
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		constraintStorage->EditAngleLimitHingeConstraint(_id).ownerEndPoint = _endPoint;
		break;
	case ConstraintType::LIMITED_BALL_JOINT:
		constraintStorage->EditLimitedBallJointConstraint(_id).ownerEndPoint = _endPoint;
		break;
	default:
		break;
	}
}

// 拘束にEndPoint追加
void PhysicsComponentAPI::AddEndPoint(ConstraintID _id, EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	PhysicsTransformID transformID;
	// エンティティに対応したTransformがあるならそれを追加ないなら何もしない
	if (transformStorage->TryGet(_entity, transformID))
	{
		switch (constraintStorage->GetType(_id))
		{
		case ConstraintType::POINTS:
			constraintStorage->EditPointConstraint(_id).endPoints.emplace_back(transformID, _localOffset, _localRotation);
			break;
		case ConstraintType::DISTANCE:
			constraintStorage->EditDistanceConstraint(_id).endPoints.emplace_back(transformID, _localOffset, _localRotation);
			break;
		case ConstraintType::HINGE:
			constraintStorage->EditHingeConstraint(_id).endPoints.emplace_back(transformID, _localOffset, _localRotation);
			break;
		case ConstraintType::ANGLE_LIMIT_POINT:
			constraintStorage->EditAngleLimitPointConstraint(_id).endPoints.emplace_back(transformID, _localOffset, _localRotation);
			break;
		case ConstraintType::ANGLE_LIMIT_HINGE:
			constraintStorage->EditAngleLimitHingeConstraint(_id).endPoints.emplace_back(transformID, _localOffset, _localRotation);
			break;
		case ConstraintType::LIMITED_BALL_JOINT:
			constraintStorage->EditLimitedBallJointConstraint(_id).endPoints.emplace_back(transformID, _localOffset, _localRotation);
			break;
		default:
			break;
		}
	}
}
// 拘束からEndPoint除外
void  PhysicsComponentAPI::RemoveEndPoint(ConstraintID _id, EntityID _entity)
{
	PhysicsTransformID transformID;
	// エンティティに対応したTransformがあるならそれを除外ないなら何もしない
	if (transformStorage->TryGet(_entity, transformID))
	{
		uint32_t index{ constraintStorage->GetDenseIndex(_id) };
		switch (constraintStorage->GetType(_id))
		{
		case ConstraintType::POINTS:
			constraintStorage->EditPointConstraint(_id).RemoveEndpoint(transformID);
			break;
		case ConstraintType::DISTANCE:
			constraintStorage->EditDistanceConstraint(_id).RemoveEndpoint(transformID);
			break;
		case ConstraintType::HINGE:
			constraintStorage->EditHingeConstraint(_id).RemoveEndpoint(transformID);
			break;
		case ConstraintType::ANGLE_LIMIT_POINT:
			constraintStorage->EditAngleLimitPointConstraint(_id).RemoveEndpoint(transformID);
			break;
		case ConstraintType::ANGLE_LIMIT_HINGE:
			constraintStorage->EditAngleLimitHingeConstraint(_id).RemoveEndpoint(transformID);
			break;
		case ConstraintType::LIMITED_BALL_JOINT:
			constraintStorage->EditLimitedBallJointConstraint(_id).RemoveEndpoint(transformID);
			break;
		default:
			break;
		}

	}
}

// Swing角度取得
float PhysicsComponentAPI::GetSwingAngle(ConstraintID _id)
{
	assert(constraintStorage->GetType(_id) == ConstraintType::LIMITED_BALL_JOINT);

	return constraintStorage->GetLimitedBallJointConstraint(_id).swingAngle;
}
// Swing角度変更
void PhysicsComponentAPI::SetSwingAngle(ConstraintID _id, float _angle)
{
	assert(constraintStorage->GetType(_id) == ConstraintType::LIMITED_BALL_JOINT);

	constraintStorage->EditLimitedBallJointConstraint(_id).swingAngle = _angle;
}
// Twist最大角度取得
float PhysicsComponentAPI::GetTwistAngleMax(ConstraintID _id)
{
	assert(constraintStorage->GetType(_id) == ConstraintType::LIMITED_BALL_JOINT);

	return constraintStorage->GetLimitedBallJointConstraint(_id).twistAngleMax;
}
// Twist最大角度変更
void PhysicsComponentAPI::SetTwistAngleMax(ConstraintID _id, float _angle)
{
	assert(constraintStorage->GetType(_id) == ConstraintType::LIMITED_BALL_JOINT);

	constraintStorage->EditLimitedBallJointConstraint(_id).twistAngleMax = _angle;
}
// Twist最小角度取得
float PhysicsComponentAPI::GetTwistAngleMin(ConstraintID _id)
{
	assert(constraintStorage->GetType(_id) == ConstraintType::LIMITED_BALL_JOINT);

	return constraintStorage->GetLimitedBallJointConstraint(_id).twistAngleMin;
}
// Twist最小角度変更
void PhysicsComponentAPI::SetTwistAngleMin(ConstraintID _id, float _angle)
{
	assert(constraintStorage->GetType(_id) == ConstraintType::LIMITED_BALL_JOINT);

	constraintStorage->EditLimitedBallJointConstraint(_id).twistAngleMin = _angle;
}

// 最小角度取得
float PhysicsComponentAPI::GetAngleMin(ConstraintID _id)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::ANGLE_LIMIT_POINT:
		return constraintStorage->GetAngleLimitPointConstraint(_id).angleMin;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		return constraintStorage->GetAngleLimitHingeConstraint(_id).angleMin;
	default:
		return 0.0f;
	}
}

// 最小角度変更
void PhysicsComponentAPI::SetAngleMin(ConstraintID _id, float _angleMin)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::ANGLE_LIMIT_POINT:
		constraintStorage->EditAngleLimitPointConstraint(_id).angleMin = _angleMin;
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		constraintStorage->EditAngleLimitHingeConstraint(_id).angleMin = _angleMin;
		break;
	default:
		break;
	}
}

// 最大角度取得
float PhysicsComponentAPI::GetAngleMax(ConstraintID _id)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::ANGLE_LIMIT_POINT:
		return constraintStorage->GetAngleLimitPointConstraint(_id).angleMax;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		return constraintStorage->GetAngleLimitHingeConstraint(_id).angleMax;
	default:
		return 0.0f;
	}
}

// 最大角度変更
void PhysicsComponentAPI::SetAngleMax(ConstraintID _id, float _angleMax)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::ANGLE_LIMIT_POINT:
		constraintStorage->EditAngleLimitPointConstraint(_id).angleMax = _angleMax;
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		constraintStorage->EditAngleLimitHingeConstraint(_id).angleMax = _angleMax;
		break;
	default:
		break;
	}
}

// 角度範囲変更
void PhysicsComponentAPI::SetAngleRange(ConstraintID _id, float _angleMin, float _angleMax)
{
	// 最小値と最大値が逆なら入れ替える
	if (_angleMin > _angleMax)
	{
		float temp{ _angleMin };
		_angleMin = _angleMax;
		_angleMax = temp;
	}

	SetAngleMin(_id, _angleMin);
	SetAngleMax(_id, _angleMax);
}

// 単一Tuning取得
ConstraintTuning PhysicsComponentAPI::GetTuning(ConstraintID _id)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::POINTS:
		return constraintStorage->GetPointConstraint(_id).tuning;
	case ConstraintType::DISTANCE:
		return constraintStorage->GetDistanceConstraint(_id).tuning;
	case ConstraintType::ANGLE_LIMIT_POINT:
		return constraintStorage->GetAngleLimitPointConstraint(_id).tuning;
	default:
		return {};
	}
}

// 単一Tuning変更
void PhysicsComponentAPI::SetTuning(ConstraintID _id, const ConstraintTuning& _tuning)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::POINTS:
		constraintStorage->EditPointConstraint(_id).tuning = _tuning;
		break;
	case ConstraintType::DISTANCE:
		constraintStorage->EditDistanceConstraint(_id).tuning = _tuning;
		break;
	case ConstraintType::ANGLE_LIMIT_POINT:
		constraintStorage->EditAngleLimitPointConstraint(_id).tuning = _tuning;
		break;
	default:
		break;
	}
}

// 位置Tuning取得
ConstraintTuning PhysicsComponentAPI::GetPositionTuning(ConstraintID _id)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::HINGE:
		return constraintStorage->GetHingeConstraint(_id).positionTuning;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		return constraintStorage->GetAngleLimitHingeConstraint(_id).positionTuning;
	case ConstraintType::LIMITED_BALL_JOINT:
		return constraintStorage->GetLimitedBallJointConstraint(_id).positionTuning;
	default:
		return {};
	}
}

// 位置Tuning変更
void PhysicsComponentAPI::SetPositionTuning(ConstraintID _id, const ConstraintTuning& _tuning)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::HINGE:
		constraintStorage->EditHingeConstraint(_id).positionTuning = _tuning;
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		constraintStorage->EditAngleLimitHingeConstraint(_id).positionTuning = _tuning;
		break;
	case ConstraintType::LIMITED_BALL_JOINT:
		constraintStorage->EditLimitedBallJointConstraint(_id).positionTuning = _tuning;
		break;
	default:
		break;
	}
}

// 回転Tuning取得
ConstraintTuning PhysicsComponentAPI::GetAngularTuning(ConstraintID _id)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::HINGE:
		return constraintStorage->GetHingeConstraint(_id).angularTuning;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		return constraintStorage->GetAngleLimitHingeConstraint(_id).angularTuning;
	case ConstraintType::LIMITED_BALL_JOINT:
		return constraintStorage->GetLimitedBallJointConstraint(_id).angularTuning;
	default:
		return {};
	}
}

// 回転Tuning変更
void PhysicsComponentAPI::SetAngularTuning(ConstraintID _id, const ConstraintTuning& _tuning)
{
	switch (constraintStorage->GetType(_id))
	{
	case ConstraintType::HINGE:
		constraintStorage->EditHingeConstraint(_id).angularTuning = _tuning;
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		constraintStorage->EditAngleLimitHingeConstraint(_id).angularTuning = _tuning;
		break;
	case ConstraintType::LIMITED_BALL_JOINT:
		constraintStorage->EditLimitedBallJointConstraint(_id).angularTuning = _tuning;
		break;
	default:
		break;
	}
}

// --- キャラクターコントローラー ---

// 作成
CharacterControllerID PhysicsComponentAPI::CreateCharacterController(EntityID _entity)
{
	if (!CanAddCharacterController(_entity))
	{
		return {};
	}

	PhysicsTransformID transformID{ transformStorage->GetOrCreateTransform(_entity) };
	ColliderID colliderID{ colliderStorage->CreateCapsule(_entity, transformID, 50.0f, 25.0f) };
	BodyID bodyID{ bodyStorage->CreateRigidBody(_entity, transformID) };

	bodyStorage->SetRigidBodyRotationLock(bodyID, RigidBodyRotationLock::ALL);

	return characterControllerStorage->Create(_entity, transformID, bodyID, colliderID);
}

// 破壊
void PhysicsComponentAPI::DestroyCharacterController(CharacterControllerID _id)
{
	if (!characterControllerStorage->IsAlive(_id))
	{
		return;
	}

	BodyID bodyID{ characterControllerStorage->GetRigidBodyID(_id) };
	ColliderID colliderID{ characterControllerStorage->GetCapsuleColliderID(_id) };

	bodyStorage->Destroy(bodyID);
	colliderStorage->Destroy(colliderID);
	characterControllerStorage->Destroy(_id);
}

// 追加可否判定
bool PhysicsComponentAPI::CanAddCharacterController(EntityID _entity)
{
	return !characterControllerStorage->Has(_entity) && CanAddBody(_entity);
}

// --- 回転 ---

// 現在の回転取得
Quaternion PhysicsComponentAPI::GetCharacterControllerRotation(CharacterControllerID _id)
{
	TransformComponent* transform{ TryGetCharacterControllerTransform(_id) };

	if (transform == nullptr)
	{
		return Quaternion::Identity();
	}

	return transform->GetRotation();
}

// 回転設定
void PhysicsComponentAPI::SetCharacterControllerRotation(CharacterControllerID _id, const Quaternion& _rotation)
{
	TransformComponent* transform{ TryGetCharacterControllerTransform(_id) };

	if (transform == nullptr)
	{
		return;
	}

	transform->SetRotation(_rotation);
	MarkColliderDirty(GetCharacterControllerColliderID(_id), AABBChangeDiaryFlag::TRANSFORM);
}

// 現在の回転へ加算
void PhysicsComponentAPI::RotateCharacterController(CharacterControllerID _id, const Quaternion& _rotation)
{
	TransformComponent* transform{ TryGetCharacterControllerTransform(_id) };

	if (transform == nullptr)
	{
		return;
	}

	transform->Rotate(_rotation);
	MarkColliderDirty(GetCharacterControllerColliderID(_id), AABBChangeDiaryFlag::TRANSFORM);
}

// --- 操作要求 ---

// ワールド空間の移動入力取得
const Vector3& PhysicsComponentAPI::GetCharacterControllerMoveInput(CharacterControllerID _id)
{
	return characterControllerStorage->GetMoveInput(_id);
}

// ワールド空間の移動入力設定
void PhysicsComponentAPI::SetCharacterControllerMoveInput(CharacterControllerID _id, const Vector3& _moveInput)
{
	characterControllerStorage->SetMoveInput(_id, _moveInput);
}

// ジャンプ要求取得
bool PhysicsComponentAPI::GetCharacterControllerJumpRequest(CharacterControllerID _id)
{
	return characterControllerStorage->GetJumpRequest(_id);
}

// ジャンプ要求設定
void PhysicsComponentAPI::SetCharacterControllerJumpRequest(CharacterControllerID _id, bool _request)
{
	characterControllerStorage->SetJumpRequest(_id, _request);
}

// 標準移動制御の有効状態取得
bool PhysicsComponentAPI::GetCharacterControllerMotorEnabled(CharacterControllerID _id)
{
	return characterControllerStorage->GetMotorEnabled(_id);
}

// 標準移動制御の有効状態設定
void PhysicsComponentAPI::SetCharacterControllerMotorEnabled(CharacterControllerID _id, bool _enabled)
{
	characterControllerStorage->SetMotorEnabled(_id, _enabled);
}

// --- Collider設定 ---

// カプセル中央部分の高さ取得
float PhysicsComponentAPI::GetCharacterControllerColliderHeight(CharacterControllerID _id)
{
	return colliderStorage->GetCapsuleColliderHeight(GetCharacterControllerColliderID(_id));
}

// カプセル中央部分の高さ設定
void PhysicsComponentAPI::SetCharacterControllerColliderHeight(CharacterControllerID _id, float _height)
{
	ColliderID colliderID{ GetCharacterControllerColliderID(_id) };

	colliderStorage->SetCapsuleColliderHeight(colliderID, _height);
	MarkColliderDirty(colliderID, AABBChangeDiaryFlag::SHAPE);
}

// カプセル半径取得
float PhysicsComponentAPI::GetCharacterControllerColliderRadius(CharacterControllerID _id)
{
	return colliderStorage->GetCapsuleColliderRadius(GetCharacterControllerColliderID(_id));
}

// カプセル半径設定
void PhysicsComponentAPI::SetCharacterControllerColliderRadius(CharacterControllerID _id, float _radius)
{
	ColliderID colliderID{ GetCharacterControllerColliderID(_id) };

	colliderStorage->SetCapsuleColliderRadius(colliderID, _radius);
	MarkColliderDirty(colliderID, AABBChangeDiaryFlag::SHAPE);
}

// --- 移動設定 ---

// 最大移動速度取得
float PhysicsComponentAPI::GetCharacterControllerMaxSpeed(CharacterControllerID _id)
{
	return characterControllerStorage->GetMaxSpeed(_id);
}

// 最大移動速度設定
void PhysicsComponentAPI::SetCharacterControllerMaxSpeed(CharacterControllerID _id, float _maxSpeed)
{
	characterControllerStorage->SetMaxSpeed(_id, _maxSpeed);
}

// 地上での最大加速度取得
float PhysicsComponentAPI::GetCharacterControllerGroundAcceleration(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundAcceleration(_id);
}

// 地上での最大加速度設定
void PhysicsComponentAPI::SetCharacterControllerGroundAcceleration(CharacterControllerID _id, float _acceleration)
{
	characterControllerStorage->SetGroundAcceleration(_id, _acceleration);
}

// 地上での最大減速度取得
float PhysicsComponentAPI::GetCharacterControllerGroundDeceleration(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundDeceleration(_id);
}

// 地上での最大減速度設定
void PhysicsComponentAPI::SetCharacterControllerGroundDeceleration(CharacterControllerID _id, float _deceleration)
{
	characterControllerStorage->SetGroundDeceleration(_id, _deceleration);
}

// 空中での最大加速度取得
float PhysicsComponentAPI::GetCharacterControllerAirAcceleration(CharacterControllerID _id)
{
	return characterControllerStorage->GetAirAcceleration(_id);
}

// 空中での最大加速度設定
void PhysicsComponentAPI::SetCharacterControllerAirAcceleration(CharacterControllerID _id, float _acceleration)
{
	characterControllerStorage->SetAirAcceleration(_id, _acceleration);
}

// ジャンプ速度取得
float PhysicsComponentAPI::GetCharacterControllerJumpSpeed(CharacterControllerID _id)
{
	return characterControllerStorage->GetJumpSpeed(_id);
}

// ジャンプ速度設定
void PhysicsComponentAPI::SetCharacterControllerJumpSpeed(CharacterControllerID _id, float _jumpSpeed)
{
	characterControllerStorage->SetJumpSpeed(_id, _jumpSpeed);
}

// 急斜面での滑り加速度取得
float PhysicsComponentAPI::GetCharacterControllerSlopeAcceleration(CharacterControllerID _id)
{
	return characterControllerStorage->GetSlopeAcceleration(_id);
}

// 急斜面での滑り加速度設定
void PhysicsComponentAPI::SetCharacterControllerSlopeAcceleration(CharacterControllerID _id, float _acceleration)
{
	characterControllerStorage->SetSlopeAcceleration(_id, _acceleration);
}

// --- 接地設定 ---

// 地面探索距離取得
float PhysicsComponentAPI::GetCharacterControllerGroundProbeDistance(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundProbeDistance(_id);
}

// 地面探索距離設定
void PhysicsComponentAPI::SetCharacterControllerGroundProbeDistance(CharacterControllerID _id, float _distance)
{
	characterControllerStorage->SetGroundProbeDistance(_id, _distance);
}

// 歩行可能判定の最小法線内積取得
float PhysicsComponentAPI::GetCharacterControllerMinGroundDot(CharacterControllerID _id)
{
	return characterControllerStorage->GetMinGroundDot(_id);
}

// 歩行可能判定の最小法線内積設定
void PhysicsComponentAPI::SetCharacterControllerMinGroundDot(CharacterControllerID _id, float _minGroundDot)
{
	characterControllerStorage->SetMinGroundDot(_id, _minGroundDot);
}

// --- 接地状態 ---

// 接地状態取得
CharacterGroundState PhysicsComponentAPI::GetCharacterControllerGroundState(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundState(_id);
}

// 検出した地面法線取得
const Vector3& PhysicsComponentAPI::GetCharacterControllerGroundNormal(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundNormal(_id);
}

// 検出した地面位置取得
const Vector3& PhysicsComponentAPI::GetCharacterControllerGroundPoint(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundPoint(_id);
}

// カプセル底面から地面までの距離取得
float PhysicsComponentAPI::GetCharacterControllerGroundDistance(CharacterControllerID _id)
{
	return characterControllerStorage->GetGroundDistance(_id);
}

// 検出した地面Collider取得
ColliderComponent PhysicsComponentAPI::GetCharacterControllerGroundCollider(CharacterControllerID _id)
{
	return ColliderComponent{ characterControllerStorage->GetGroundColliderID(_id) };
}

// --- 内部計算用生成関数 ---

// 内部用のPhysicsTransform作成(寿命管理をちゃんを忘れない)
PhysicsTransformID PhysicsComponentAPI::CreateInternalPhysicsTransformID(EntityID _entity, const Vector3& _position, const Quaternion& _rotation, const Vector3& _scale)
{
	return transformStorage->CreateInternalTransform(_entity, _position, _rotation, _scale);
}

// 内部用のRigidBody作成(寿命管理をちゃんを忘れない)
BodyID PhysicsComponentAPI::CreateInternalRigidBody(EntityID _entity, PhysicsTransformID _transformID)
{
	return bodyStorage->CreateRigidBody(_entity, _transformID);
}

// 内部用の球コライダー作成(寿命管理をちゃんを忘れない)
ColliderID PhysicsComponentAPI::CreateInternalSphereCollider(EntityID _entity, PhysicsTransformID _transformID, float _radius)
{
	return colliderStorage->CreateSphere(_entity, _transformID, _radius);
}

// 内部用のカプセルコライダー作成(寿命管理をちゃんを忘れない)
ColliderID PhysicsComponentAPI::CreateInternalCapsuleCollider(EntityID _entity, PhysicsTransformID _transformID, float _height, float _radius)
{
	return colliderStorage->CreateCapsule(_entity, _transformID, _height, _radius);
}

// 内部用のボックスコライダー作成(寿命管理をちゃんを忘れない)
ColliderID PhysicsComponentAPI::CreateInternalBoxCollider(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale)
{
	return colliderStorage->CreateBox(_entity, _transformID, _scale);
}

// 内部用の点拘束作成(寿命管理をちゃんを忘れない)
ConstraintID PhysicsComponentAPI::CreateInternalPointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset)
{
	return constraintStorage->CreatePointConstraint(_entity, _transformID, _localOffset);
}

// 内部用の距離拘束作成(寿命管理をちゃんを忘れない)
ConstraintID PhysicsComponentAPI::CreateInternalDistanceConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset)
{
	return constraintStorage->CreateDistanceConstraint(_entity, _transformID, _localOffset);
}

// 内部用のヒンジ拘束作成(寿命管理をちゃんを忘れない)
ConstraintID PhysicsComponentAPI::CreateInternalHingeConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateHingeConstraint(_entity, _transformID, _localOffset, _localRotation);
}

// 内部用の角度制限付き点拘束作成(寿命管理をちゃんを忘れない)
ConstraintID PhysicsComponentAPI::CreateInternalAngleLimitPointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateAngleLimitPointConstraint(_entity, _transformID, _localOffset, _localRotation);
}

// 内部用の角度制限付きヒンジ拘束作成(寿命管理をちゃんを忘れない)
ConstraintID PhysicsComponentAPI::CreateInternalAngleLimitHingeConstraint(
	EntityID _entity, PhysicsTransformID _transformID,
	const Vector3& _localOffset, const Quaternion& _localRotation)
{

	return constraintStorage->CreateAngleLimitHingeConstraint(
		_entity, transformStorage->GetOrCreateTransform(_entity),
		_localOffset, _localRotation);
}

// SwingTwist拘束作成関数(寿命管理をちゃんを忘れない)
ConstraintID PhysicsComponentAPI::CreateInternalLimitedBallJointConstraint(
	EntityID _entity, PhysicsTransformID _transformID,
	const Vector3& _localOffset, const Quaternion& _localRotation)
{
	return constraintStorage->CreateLimitedBallJointConstraint(
		_entity, _transformID,
		_localOffset, _localRotation
	);
}


// 内部用拘束のEndPoint追加(寿命管理をちゃんを忘れない)
void PhysicsComponentAPI::AddInternalEndPoint(ConstraintID _constraintID, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation)
{
	switch (constraintStorage->GetType(_constraintID))
	{
	case ConstraintType::POINTS:
		constraintStorage->EditPointConstraint(_constraintID).endPoints.emplace_back(_transformID, _localOffset, _localRotation);
		break;
	case ConstraintType::DISTANCE:
		constraintStorage->EditDistanceConstraint(_constraintID).endPoints.emplace_back(_transformID, _localOffset, _localRotation);
		break;
	case ConstraintType::HINGE:
		constraintStorage->EditHingeConstraint(_constraintID).endPoints.emplace_back(_transformID, _localOffset, _localRotation);
		break;
	case ConstraintType::ANGLE_LIMIT_POINT:
		constraintStorage->EditAngleLimitPointConstraint(_constraintID).endPoints.emplace_back(_transformID, _localOffset, _localRotation);
		break;
	case ConstraintType::ANGLE_LIMIT_HINGE:
		constraintStorage->EditAngleLimitHingeConstraint(_constraintID).endPoints.emplace_back(_transformID, _localOffset, _localRotation);
		break;
	default:
		break;
	}
}

// PhysicsTransform破棄(対応する他の奴も破棄する)
void PhysicsComponentAPI::DestroyPhysicsTransform(PhysicsTransformID _transformID)
{
	BodyID bodyID;
	if (bodyStorage->TryGetRigidBodyID(_transformID, bodyID))
	{
		bodyStorage->Destroy(bodyID);
	}
	
	for (auto colliderID : colliderStorage->GetColliderIDFromTransformID(_transformID))
	{
		colliderStorage->Destroy(colliderID);
	}

	ConstraintID constraintID;
	if (constraintStorage->TryGetConstraintIDFromTransformID(_transformID, constraintID))
	{
		constraintStorage->Destory(constraintID);
	}

	transformStorage->Destroy(_transformID);
}

// --- World接続 ---

void PhysicsComponentAPI::BindWorld(PhysicsWorld& _physicsWorld, WorldStorage& _componentWorld)
{
	physicsWorld = &_physicsWorld;
	componentWorld = &_componentWorld;
	colliderStorage = _physicsWorld.GetColliderStorage();
	bodyStorage = _physicsWorld.GetRigidBodyStorage();
	transformStorage = _physicsWorld.GetPhysicsTransformStorage();
	constraintStorage = _physicsWorld.GetConstraintStorage();
	characterControllerStorage = _physicsWorld.GetCharacterControllerStorage();
}

void PhysicsComponentAPI::UnbindWorld()
{
	componentWorld = nullptr;
	physicsWorld = nullptr;
	colliderStorage = nullptr;
	bodyStorage = nullptr;
	transformStorage = nullptr;
	constraintStorage = nullptr;
	characterControllerStorage = nullptr;
}

// --- private ---

// Collider生存確認
bool PhysicsComponentAPI::IsColliderAlive(ColliderID _id)
{
	return colliderStorage != nullptr && _id.IsValid() && colliderStorage->IsAlive(_id);
}

// Collider種別取得
ColliderType PhysicsComponentAPI::GetColliderType(ColliderID _id)
{
	if (!IsColliderAlive(_id))
	{
		return ColliderType::COUNT;
	}

	return colliderStorage->GetType(_id);
}

// CharacterController内部Collider取得
ColliderID PhysicsComponentAPI::GetCharacterControllerColliderID(CharacterControllerID _id)
{
	return characterControllerStorage->GetCapsuleColliderID(_id);
}

// CharacterControllerのTransform取得
TransformComponent* PhysicsComponentAPI::TryGetCharacterControllerTransform(CharacterControllerID _id)
{
	if (componentWorld == nullptr ||
		characterControllerStorage == nullptr ||
		!characterControllerStorage->IsAlive(_id))
	{
		return nullptr;
	}

	ComponentStorageBase<TransformComponent>* storage{ componentWorld->GetStorage<TransformComponent>() };

	if (storage == nullptr)
	{
		return nullptr;
	}

	return storage->TryEdit(characterControllerStorage->GetOwnerEntity(_id));
}

// ColliderのAABB変更通知
void PhysicsComponentAPI::MarkColliderDirty(ColliderID _id, AABBChangeDiaryFlag _flag)
{
	if (colliderStorage == nullptr ||
		!colliderStorage->IsAlive(_id))
	{
		return;
	}

	uint32_t index{ colliderStorage->GetAABBIndex(_id) };
	colliderStorage->EditAABBDiaryFlag(index) |= _flag;
}

WorldStorage* PhysicsComponentAPI::componentWorld{ nullptr };
PhysicsWorld* PhysicsComponentAPI::physicsWorld{ nullptr };
ColliderStorage* PhysicsComponentAPI::colliderStorage{ nullptr };
BodyStorage* PhysicsComponentAPI::bodyStorage{ nullptr };
PhysicsTransformStorage* PhysicsComponentAPI::transformStorage{ nullptr };
ConstraintStorage* PhysicsComponentAPI::constraintStorage{ nullptr };
CharacterControllerStorage* PhysicsComponentAPI::characterControllerStorage{ nullptr };
