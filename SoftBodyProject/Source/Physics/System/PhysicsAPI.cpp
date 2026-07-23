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
	bodyStorage->rigidBodyStorage->force[bodyStorage->GetDenseIndex(_id)] += _force;
}
// トルク加算
void PhysicsAPI::AddTorque(BodyID _id, const Vector3& _torque)
{
	// 加算
	bodyStorage->rigidBodyStorage->torque[bodyStorage->GetDenseIndex(_id)] += _torque;
}
// 速度取得
const Vector3& PhysicsAPI::GetVelocity(BodyID _id)
{
	return bodyStorage->rigidBodyStorage->velocity[bodyStorage->GetDenseIndex(_id)];
}
// 速度変更
void PhysicsAPI::SetVelocity(BodyID _id, const Vector3& _velocity)
{
	bodyStorage->rigidBodyStorage->velocity[bodyStorage->GetDenseIndex(_id)] = _velocity;
}
// 角速度取得
const Vector3& PhysicsAPI::GetAngularVelocity(BodyID _id)
{
	return bodyStorage->rigidBodyStorage->angularVelocity[bodyStorage->GetDenseIndex(_id)];
}
// 角速度変更
void PhysicsAPI::SetAngularVelocity(BodyID _id, const Vector3& _omega)
{
	bodyStorage->rigidBodyStorage->angularVelocity[bodyStorage->GetDenseIndex(_id)] = _omega;
}
// 質量取得
float PhysicsAPI::GetMass(BodyID _id)
{
	return bodyStorage->rigidBodyStorage->mass[bodyStorage->GetDenseIndex(_id)];
}
// 質量変更
void PhysicsAPI::SetMass(BodyID _id, float _mass)
{
	if (_mass <= 0)
	{
		return;
	}

	bodyStorage->rigidBodyStorage->mass[bodyStorage->GetDenseIndex(_id)] = _mass;
	bodyStorage->rigidBodyStorage->inverseMass[bodyStorage->GetDenseIndex(_id)] = 1 / _mass;
	bodyStorage->rigidBodyStorage->localInertiaDirty[bodyStorage->GetDenseIndex(_id)] = true;
}
// 慣性テンソル取得
const Matrix4x4& PhysicsAPI::GetInertiaTensor(BodyID _id)
{
	return bodyStorage->rigidBodyStorage->inertiaTensor[bodyStorage->GetDenseIndex(_id)];
}
// 慣性テンソル変更
void PhysicsAPI::SetInertiaTensor(BodyID _id, Matrix4x4& _matrix)
{
	bodyStorage->rigidBodyStorage->inertiaTensor[bodyStorage->GetDenseIndex(_id)] = _matrix;
	bodyStorage->rigidBodyStorage->localInverseInertiaTensor[bodyStorage->GetDenseIndex(_id)] = NamericalAnalysis::GaussJordan(_matrix);
	bodyStorage->rigidBodyStorage->localInertiaDirty[bodyStorage->GetDenseIndex(_id)] = true;
}
// isGravity取得
bool PhysicsAPI::GetIsGravity(BodyID _id)
{
	return bodyStorage->rigidBodyStorage->isGravity[bodyStorage->GetDenseIndex(_id)];
}
// isGravity変更
void PhysicsAPI::SetIsGravity(BodyID _id, bool _isGravity)
{
	bodyStorage->rigidBodyStorage->isGravity[bodyStorage->GetDenseIndex(_id)] = _isGravity;
}
// 重力加速度取得
const Vector3& PhysicsAPI::GetGravity(BodyID _id)
{
	return bodyStorage->rigidBodyStorage->gravity[bodyStorage->GetDenseIndex(_id)];
}
// 重力加速度変更
void PhysicsAPI::SetGravity(BodyID _id, const Vector3& _gravity)
{
	bodyStorage->rigidBodyStorage->gravity[bodyStorage->GetDenseIndex(_id)] = _gravity;
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
	return bodyStorage->ropeStorage->length[bodyStorage->GetDenseIndex(_id)];
}
// 全体の長さ変更
void PhysicsAPI::SetLength(BodyID _id, float _length)
{
	bodyStorage->ropeStorage->length[bodyStorage->GetDenseIndex(_id)] = _length;
}
// 分割数取得
int PhysicsAPI::GetSegmentCount(BodyID _id)
{
	return bodyStorage->ropeStorage->segmentCount[bodyStorage->GetDenseIndex(_id)];
}
// 分割数変更
void PhysicsAPI::SetSegmentCount(BodyID _id, int _segmentCount)
{
	bodyStorage->ropeStorage->segmentCount[bodyStorage->GetDenseIndex(_id)] = _segmentCount;
}

// --- クロス系 ---

// 横幅取得
float PhysicsAPI::GetClothWidth(BodyID _id)
{
	return bodyStorage->clothStorage->width[bodyStorage->GetDenseIndex(_id)];
}
// 横幅変更
void PhysicsAPI::SetClothWidth(BodyID _id, float _width)
{
	bodyStorage->clothStorage->width[bodyStorage->GetDenseIndex(_id)] = _width;
}
// 縦幅取得
float PhysicsAPI::GetClothHeight(BodyID _id)
{
	return bodyStorage->clothStorage->height[bodyStorage->GetDenseIndex(_id)];
}
// 縦幅変更
void PhysicsAPI::SetClothHeight(BodyID _id, float _height)
{
	bodyStorage->clothStorage->height[bodyStorage->GetDenseIndex(_id)] = _height;
}
// 横分割取得
int PhysicsAPI::GetRowCount(BodyID _id)
{
	return bodyStorage->clothStorage->rowCount[bodyStorage->GetDenseIndex(_id)];
}
// 横分割変更
void PhysicsAPI::SetRowCount(BodyID _id, int _rowCount)
{
	bodyStorage->clothStorage->rowCount[bodyStorage->GetDenseIndex(_id)] = _rowCount;
}
// 縦分割取得
int PhysicsAPI::GetColumnCount(BodyID _id)
{
	return bodyStorage->clothStorage->columnCount[bodyStorage->GetDenseIndex(_id)];
}
// 縦分割変更
void PhysicsAPI::SetColumnCount(BodyID _id, int _columnCount)
{
	bodyStorage->clothStorage->columnCount[bodyStorage->GetDenseIndex(_id)] = _columnCount;
}

// --- ソフトボディ系 ---

// 横幅取得
float PhysicsAPI::GetSoftBodyWidth(BodyID _id)
{
	return bodyStorage->softBodyStorage->width[bodyStorage->GetDenseIndex(_id)];
}
// 横幅変更
void PhysicsAPI::SetSoftBodyWidth(BodyID _id, float _width)
{
	bodyStorage->softBodyStorage->width[bodyStorage->GetDenseIndex(_id)] = _width;
}
// 縦幅取得
float PhysicsAPI::GetSoftBodyHeight(BodyID _id)
{
	return bodyStorage->softBodyStorage->height[bodyStorage->GetDenseIndex(_id)];
}
// 縦幅変更
void PhysicsAPI::SetSoftBodyHeight(BodyID _id, float _height)
{
	bodyStorage->softBodyStorage->height[bodyStorage->GetDenseIndex(_id)] = _height;
}
// 奥行き取得
float PhysicsAPI::GetSoftBodyDepth(BodyID _id)
{
	return bodyStorage->softBodyStorage->depth[bodyStorage->GetDenseIndex(_id)];
}
// 奥行き変更
void PhysicsAPI::SetSoftBodyDepth(BodyID _id, float _depth)
{
	bodyStorage->softBodyStorage->depth[bodyStorage->GetDenseIndex(_id)] = _depth;
}
// 横分割取得
int PhysicsAPI::GetSegmentCountX(BodyID _id)
{
	return bodyStorage->softBodyStorage->segmentCountX[bodyStorage->GetDenseIndex(_id)];
}
// 横分割変更
void PhysicsAPI::SetSegmentCountX(BodyID _id, int _segmentCountX)
{
	bodyStorage->softBodyStorage->segmentCountX[bodyStorage->GetDenseIndex(_id)] = _segmentCountX;
}
// 縦分割取得
int PhysicsAPI::GetSegmentCountY(BodyID _id)
{
	return bodyStorage->softBodyStorage->segmentCountY[bodyStorage->GetDenseIndex(_id)];
}
// 縦分割変更
void PhysicsAPI::SetSegmentCountY(BodyID _id, int _segmentCountY)
{
	bodyStorage->softBodyStorage->segmentCountY[bodyStorage->GetDenseIndex(_id)] = _segmentCountY;
}
// 奥行き分割取得
int PhysicsAPI::GetSegmentCountZ(BodyID _id)
{
	return bodyStorage->softBodyStorage->segmentCountZ[bodyStorage->GetDenseIndex(_id)];
}
// 奥行き分割変更
void PhysicsAPI::SetSegmentCountZ(BodyID _id, int _segmentCountZ)
{
	bodyStorage->softBodyStorage->segmentCountZ[bodyStorage->GetDenseIndex(_id)] = _segmentCountZ;
}

// --- コライダー系 ---

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
	return colliderStorage->sphereStorage->radius[colliderStorage->GetDenseIndex(_id)];
}
// 半径変更
void PhysicsAPI::SetRadius(ColliderID _id, float _radius)
{
	colliderStorage->sphereStorage->radius[colliderStorage->GetDenseIndex(_id)] = _radius;
}
// X方向の長さ取得
float PhysicsAPI::GetWidth(ColliderID _id)
{
	return colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].x;
}
// X方向の長さ変更
void PhysicsAPI::SetWidth(ColliderID _id, float _width)
{
	colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].x = _width;
}
// Y方向の長さ取得
float PhysicsAPI::GetHeight(ColliderID _id)
{
	return colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].y;
}
// Y方向の長さ変更
void PhysicsAPI::SetHeight(ColliderID _id, float _height)
{
	colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].y = _height;
}
// Z方向の長さ取得
float PhysicsAPI::GetDepth(ColliderID _id)
{
	return colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].z;
}
// Z方向の長さ変更
void PhysicsAPI::SetDepth(ColliderID _id, float _depth)
{
	colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].z = _depth;
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

// 距離拘束の距離取得
float PhysicsAPI::GetDistance(ConstraintID _id)
{
	return constraintStorage->distanceConstraintStorage->constraints[_id.GetIndex()].distance;
}
// 距離拘束の距離設定
void PhysicsAPI::SetDistance(ConstraintID _id, float _distance)
{
	constraintStorage->distanceConstraintStorage->constraints[_id.GetIndex()].distance = _distance;
}
// 拘束にEndPoint追加
void PhysicsAPI::AddEndPoint(ConstraintID _id, EntityID _entity, const Vector3& _localOffset)
{
	PhysicsTransformID transformID;
	// エンティティに対応したTransformがあるならそれを追加ないなら何もしない
	if (transformStorage->TryGet(_entity, transformID))
	{
		uint32_t index{ constraintStorage->GetDenseIndex(_id) };
		switch (constraintStorage->GetType(_id))
		{
		case ConstraintType::POINTS:
			constraintStorage->pointConstraintStorage->constraints[index].endPoints.emplace_back(transformID, _localOffset);
			break;
		case ConstraintType::DISTANCE:
			constraintStorage->distanceConstraintStorage->constraints[index].endPoints.emplace_back(transformID, _localOffset);
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
			constraintStorage->pointConstraintStorage->constraints[index].RemoveEndpoint(transformID);
			break;
		case ConstraintType::DISTANCE:
			constraintStorage->distanceConstraintStorage->constraints[index].RemoveEndpoint(transformID);
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
