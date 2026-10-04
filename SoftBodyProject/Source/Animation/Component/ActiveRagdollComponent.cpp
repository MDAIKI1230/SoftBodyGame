#include "AnimationComponentAPI.h"

#include "ActiveRagdollComponent.h"

ActiveRagdollComponent::ActiveRagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const std::string& _path)
{
	id = AnimationComponentAPI::CreateActiveRagdoll(_entity, rendererComponent, _path);
}

// --- Body取得 ---

// 指定した役割のRigidBody取得
RigidBodyComponent ActiveRagdollComponent::GetBody(RagdollBoneRole _role) const
{
	return AnimationComponentAPI::GetActiveRagdollBody(id, _role);
}

// 名前からボーンインデックス取得
uint32_t ActiveRagdollComponent::GetBoneIndex(const char* _boneName) const
{
	return AnimationComponentAPI::GetActiveRagdollBoneIndex(id, _boneName);
}

// 役割からボーンインデックス取得
uint32_t ActiveRagdollComponent::GetBoneIndex(RagdollBoneRole _role) const
{
	return AnimationComponentAPI::GetActiveRagdollBoneIndex(id, _role);
}

// インデックス指定で個別変更
bool ActiveRagdollComponent::SetWeight(uint32_t _boneIndex, float _weight)
{
	return AnimationComponentAPI::SetActiveRagdollWeight(id, _boneIndex, _weight);
}

// 役割指定で個別変更
bool ActiveRagdollComponent::SetWeight(RagdollBoneRole _role, float _weight)
{
	return AnimationComponentAPI::SetActiveRagdollWeight(id, _role, _weight);
}

// インデックス指定で自身と子孫を変更
bool ActiveRagdollComponent::SetBranchWeight(uint32_t _rootBoneIndex, float _weight)
{
	return AnimationComponentAPI::SetActiveRagdollBranchWeight(id, _rootBoneIndex, _weight);
}

// 役割指定で自身と子孫を変更
bool ActiveRagdollComponent::SetBranchWeight(RagdollBoneRole _rootRole, float _weight)
{
	return AnimationComponentAPI::SetActiveRagdollBranchWeight(id, _rootRole, _weight);
}

// --- 操作要求 ---

// ワールド空間の移動入力取得
const Vector3& ActiveRagdollComponent::GetMoveInput() const
{
	return AnimationComponentAPI::GetActiveRagdollMoveInput(id);
}

// ワールド空間の移動入力設定
void ActiveRagdollComponent::SetMoveInput(const Vector3& _moveInput)
{
	AnimationComponentAPI::SetActiveRagdollMoveInput(id, _moveInput);
}

// 移動入力クリア
void ActiveRagdollComponent::ClearMoveInput()
{
	AnimationComponentAPI::SetActiveRagdollMoveInput(id, Vector3::ZERO);
}

// ジャンプ要求
void ActiveRagdollComponent::RequestJump()
{
	AnimationComponentAPI::RequestActiveRagdollJump(id);
}

// --- 状態取得 ---

// 制御状態取得
ActiveRagdollControlState ActiveRagdollComponent::GetControlState() const
{
	return AnimationComponentAPI::GetActiveRagdollControlState(id);
}

// 接地状態取得
bool ActiveRagdollComponent::IsGrounded() const
{
	return AnimationComponentAPI::GetActiveRagdollGrounded(id);
}

// 地面法線取得
const Vector3& ActiveRagdollComponent::GetGroundNormal() const
{
	return AnimationComponentAPI::GetActiveRagdollGroundNormal(id);
}

// 水平速度取得
const Vector3& ActiveRagdollComponent::GetPlanarVelocity() const
{
	return AnimationComponentAPI::GetActiveRagdollPlanarVelocity(id);
}

// 重心位置取得
const Vector3& ActiveRagdollComponent::GetCenterOfMass() const
{
	return AnimationComponentAPI::GetActiveRagdollCenterOfMass(id);
}

// 直立度取得
float ActiveRagdollComponent::GetUprightDot() const
{
	return AnimationComponentAPI::GetActiveRagdollUprightDot(id);
}

// Ragdoll全体を無視する衝突フィルター取得
CollisionFilter ActiveRagdollComponent::GetIgnoreFilter() const
{
	return AnimationComponentAPI::GetActiveRagdollIgnoreFilter(id);
}