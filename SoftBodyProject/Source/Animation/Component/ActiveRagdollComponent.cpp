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