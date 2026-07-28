#include "NamericalAnalysis.h"

#include "RigidBodyComponentStorage.h"
#include "BoxColliderComponentStorage.h"

#include "PhysicsAPI.h"

// --- Body系 ---

// 追加可否判定
bool PhysicsAPI::CanAddBody(EntityID _entity)
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
void PhysicsAPI::DestroyBody(BodyID _id)
{
	bodyStorage->Destroy(_id);
}
// --- RigidBody ---

// 作成
BodyID PhysicsAPI::CreateRigidBody(EntityID _entity)
{
	return bodyStorage->CreateRigidBody(_entity, transformStorage->GetOrCreateTransform(_entity));
}
// 力加算
void PhysicsAPI::AddForce(BodyID _id, const Vector3& _force)
{
	// 加算
	bodyStorage->EditRigidBodyForce(_id) += _force;
}
// トルク加算
void PhysicsAPI::AddTorque(BodyID _id, const Vector3& _torque)
{
	// 加算
	bodyStorage->EditRigidBodyTorque(_id) += _torque;
}
// 速度取得
const Vector3& PhysicsAPI::GetVelocity(BodyID _id)
{
	return bodyStorage->GetRigidBodyVelocity(_id);
}
// 速度変更
void PhysicsAPI::SetVelocity(BodyID _id, const Vector3& _velocity)
{
	bodyStorage->SetRigidBodyVelocity(_id, _velocity);
}
// 角速度取得
const Vector3& PhysicsAPI::GetAngularVelocity(BodyID _id)
{
	return bodyStorage->GetRigidBodyAngularVelocity(_id);
}
// 角速度変更
void PhysicsAPI::SetAngularVelocity(BodyID _id, const Vector3& _omega)
{
	bodyStorage->SetRigidBodyAngularVelocity(_id, _omega);
}
// isGravity取得
bool PhysicsAPI::GetIsGravity(BodyID _id)
{
	return bodyStorage->GetRigidBodyIsGravity(_id);
}
// isGravity変更
void PhysicsAPI::SetIsGravity(BodyID _id, bool _isGravity)
{
	bodyStorage->SetRigidBodyIsGravity(_id, _isGravity);
}
// 重力加速度取得
const Vector3& PhysicsAPI::GetGravity(BodyID _id)
{
	return bodyStorage->GetRigidBodyGravity(_id);
}
// 重力加速度変更
void PhysicsAPI::SetGravity(BodyID _id, const Vector3& _gravity)
{
	bodyStorage->SetRigidBodyGravity(_id, _gravity);
}
// 質量取得
float PhysicsAPI::GetMass(BodyID _id)
{
	return bodyStorage->GetRigidBodyMass(_id);
}
// 質量変更
void PhysicsAPI::SetMass(BodyID _id, float _mass)
{
	bodyStorage->SetRigidBodyMass(_id, _mass);
}

// --- ソフト系 ---

// ロープ作成
BodyID PhysicsAPI::CreateRope(EntityID _entity, const RopeUpdateInfo _info)
{
	return bodyStorage->CreateRope(_entity, transformStorage->GetOrCreateTransform(_entity), _info);
}
// クロス作成
BodyID PhysicsAPI::CreateCloth(EntityID _entity, const ClothUpdateInfo _info)
{
	return bodyStorage->CreateCloth(_entity, transformStorage->GetOrCreateTransform(_entity), _info);
}
// ソフトボディ作成
BodyID PhysicsAPI::CreateSoftBody(EntityID _entity, const SoftBodyUpdateInfo _info)
{
	return bodyStorage->CreateSoftBody(_entity, transformStorage->GetOrCreateTransform(_entity), _info);
}

// --- ロープ系 ---

// 全体の長さ取得
float PhysicsAPI::GetLength(BodyID _id)
{
	return bodyStorage->GetRopeLength(_id);
}
// 全体の長さ変更
void PhysicsAPI::SetLength(BodyID _id, float _length)
{
	bodyStorage->SetRopeLength(_id, _length);
}
// 分割数取得
int PhysicsAPI::GetSegmentCount(BodyID _id)
{
	return bodyStorage->GetRopeSegmentCount(_id);
}
// 分割数変更
void PhysicsAPI::SetSegmentCount(BodyID _id, int _segmentCount)
{
	bodyStorage->SetRopeSegmentCount(_id, _segmentCount);
}

// --- クロス系 ---

// 横幅取得
float PhysicsAPI::GetClothWidth(BodyID _id)
{
	return bodyStorage->GetClothWidth(_id);
}
// 横幅変更
void PhysicsAPI::SetClothWidth(BodyID _id, float _width)
{
	bodyStorage->SetClothWidth(_id, _width);
}
// 縦幅取得
float PhysicsAPI::GetClothHeight(BodyID _id)
{
	return bodyStorage->GetClothHeight(_id);
}
// 縦幅変更
void PhysicsAPI::SetClothHeight(BodyID _id, float _height)
{
	bodyStorage->SetClothHeight(_id, _height);
}
// 横分割取得
int PhysicsAPI::GetRowCount(BodyID _id)
{
	return bodyStorage->GetClothRowCount(_id);
}
// 横分割変更
void PhysicsAPI::SetRowCount(BodyID _id, int _rowCount)
{
	bodyStorage->SetClothRowCount(_id, _rowCount);
}
// 縦分割取得
int PhysicsAPI::GetColumnCount(BodyID _id)
{
	return bodyStorage->GetClothColumnCount(_id);
}
// 縦分割変更
void PhysicsAPI::SetColumnCount(BodyID _id, int _columnCount)
{
	bodyStorage->SetClothColumnCount(_id, _columnCount);
}

// --- ソフトボディ系 ---

// 横幅取得
float PhysicsAPI::GetSoftBodyWidth(BodyID _id)
{
	return bodyStorage->GetSoftBodyWidth(_id);
}
// 横幅変更
void PhysicsAPI::SetSoftBodyWidth(BodyID _id, float _width)
{
	bodyStorage->SetSoftBodyWidth(_id, _width);
}
// 縦幅取得
float PhysicsAPI::GetSoftBodyHeight(BodyID _id)
{
	return bodyStorage->GetSoftBodyHeight(_id);
}
// 縦幅変更
void PhysicsAPI::SetSoftBodyHeight(BodyID _id, float _height)
{
	bodyStorage->SetSoftBodyHeight(_id, _height);
}
// 奥行き取得
float PhysicsAPI::GetSoftBodyDepth(BodyID _id)
{
	return bodyStorage->GetSoftBodyDepth(_id);
}
// 奥行き変更
void PhysicsAPI::SetSoftBodyDepth(BodyID _id, float _depth)
{
	bodyStorage->SetSoftBodyDepth(_id, _depth);
}
// 横分割取得
int PhysicsAPI::GetSegmentCountX(BodyID _id)
{
	return bodyStorage->GetSoftBodySegmentCountX(_id);
}
// 横分割変更
void PhysicsAPI::SetSegmentCountX(BodyID _id, int _segmentCountX)
{
	bodyStorage->SetSoftBodySegmentCountX(_id, _segmentCountX);
}
// 縦分割取得
int PhysicsAPI::GetSegmentCountY(BodyID _id)
{
	return bodyStorage->GetSoftBodySegmentCountY(_id);
}
// 縦分割変更
void PhysicsAPI::SetSegmentCountY(BodyID _id, int _segmentCountY)
{
	bodyStorage->SetSoftBodySegmentCountY(_id, _segmentCountY);
}
// 奥行き分割取得
int PhysicsAPI::GetSegmentCountZ(BodyID _id)
{
	return bodyStorage->GetSoftBodySegmentCountZ(_id);
}
// 奥行き分割変更
void PhysicsAPI::SetSegmentCountZ(BodyID _id, int _segmentCountZ)
{
	bodyStorage->SetSoftBodySegmentCountZ(_id, _segmentCountZ);
}

// --- コライダー系 ---

void PhysicsAPI::DestroyCollider(ColliderID _id)
{
	colliderStorage->Destroy(_id);
}

// 球作成
ColliderID PhysicsAPI::CreateSphere(EntityID _entity, float _radius)
{
	return colliderStorage->CreateSphere(_entity, transformStorage->GetOrCreateTransform(_entity), _radius);
}
// Box作成
ColliderID PhysicsAPI::CreateBox(EntityID _entity, const Vector3& _scale)
{
	return colliderStorage->CreateBox(_entity, transformStorage->GetOrCreateTransform(_entity), _scale);
}
// 半径取得
float PhysicsAPI::GetRadius(ColliderID _id)
{
	return colliderStorage->GetSphereColliderRadius(_id);
}
// 半径変更
void PhysicsAPI::SetRadius(ColliderID _id, float _radius)
{
	colliderStorage->SetSphereColliderRadius(_id, _radius);
}
// X方向の長さ取得
float PhysicsAPI::GetWidth(ColliderID _id)
{
	return colliderStorage->GetBoxColliderScale(_id).x;
}
// X方向の長さ変更
void PhysicsAPI::SetWidth(ColliderID _id, float _width)
{
	colliderStorage->EditBoxColliderScale(_id).x = _width;
}
// Y方向の長さ取得
float PhysicsAPI::GetHeight(ColliderID _id)
{
	return colliderStorage->GetBoxColliderScale(_id).y;
}
// Y方向の長さ変更
void PhysicsAPI::SetHeight(ColliderID _id, float _height)
{
	colliderStorage->EditBoxColliderScale(_id).y = _height;
}
// Z方向の長さ取得
float PhysicsAPI::GetDepth(ColliderID _id)
{
	return colliderStorage->GetBoxColliderScale(_id).z;
}
// Z方向の長さ変更
void PhysicsAPI::SetDepth(ColliderID _id, float _depth)
{
	colliderStorage->EditBoxColliderScale(_id).z = _depth;
}

// --- 拘束系 ---

// 点拘束作成
ConstraintID PhysicsAPI::CreatePointConstraint(EntityID _entity, const Vector3& _localOffset)
{
	return constraintStorage->CreatePointConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset);
}
// 距離拘束作成
ConstraintID PhysicsAPI::CreateDistanceConstraint(EntityID _entity, const Vector3& _localOffset, float _distance)
{
	return constraintStorage->CreateDistanceConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset, _distance);
}

// 破壊
void PhysicsAPI::DestroyConstraint(ConstraintID _id)
{
	constraintStorage->Destory(_id);
}

// 距離拘束の距離取得
float PhysicsAPI::GetDistance(ConstraintID _id)
{
	return constraintStorage->GetDistanceConstraint(_id).distance;
}
// 距離拘束の距離設定
void PhysicsAPI::SetDistance(ConstraintID _id, float _distance)
{
	constraintStorage->EditDistanceConstraint(_id).distance = _distance;
}
// 拘束にEndPoint追加
void PhysicsAPI::AddEndPoint(ConstraintID _id, EntityID _entity, const Vector3& _localOffset)
{
	PhysicsTransformID transformID;
	// エンティティに対応したTransformがあるならそれを追加ないなら何もしない
	if (transformStorage->TryGet(_entity, transformID))
	{
		switch (constraintStorage->GetType(_id))
		{
		case ConstraintType::POINTS:
			constraintStorage->EditPointConstraint(_id).endPoints.emplace_back(transformID, _localOffset);
			break;
		case ConstraintType::DISTANCE:
			constraintStorage->EditDistanceConstraint(_id).endPoints.emplace_back(transformID, _localOffset);
			break;
		default:
			break;
		}
	}
}
// 拘束からEndPoint除外
void  PhysicsAPI::RemoveEndPoint(ConstraintID _id, EntityID _entity)
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
		default:
			break;
		}
		
	}
}

PhysicsWorld* PhysicsAPI::physicsWorld{ nullptr };
ColliderStorage* PhysicsAPI::colliderStorage{ nullptr };
BodyStorage* PhysicsAPI::bodyStorage{ nullptr };
PhysicsTransformStorage* PhysicsAPI::transformStorage{ nullptr };
ConstraintStorage* PhysicsAPI::constraintStorage{ nullptr };
