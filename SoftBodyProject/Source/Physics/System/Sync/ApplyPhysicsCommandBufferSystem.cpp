#include "ApplyPhysicsCommandBufferSystem.h"

// 適用関数
void ApplyPhysicsCommandBufferSystem::Apply(PhysicsCommandBuffer* _commandBuffer, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
	ApplyBodyCommand(_commandBuffer, _transformStorage, _bodyStorage);
}

// リジッドボディの更新
void ApplyPhysicsCommandBufferSystem::ApplyBodyCommand(PhysicsCommandBuffer* _commandBuffer, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
	for (auto index : _commandBuffer->GetRecordedIndices())
	{
		const BodyUpdate& update{ _commandBuffer->GetBodyUpdate(index) };

		BodyID bodyID{ update.target };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };

		if (HasFlag(update.flags, RigidBodyUpdateFlag::POSITION))
		{
			_transformStorage->EditPosition(transIndex) = update.position;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::VELOCITY))
		{
			_bodyStorage->EditRigidBodyVelocity(bodyID) = update.velocity;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::FORCE))
		{
			_bodyStorage->EditRigidBodyForce(bodyID) = update.force;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::ROTATION))
		{
			_transformStorage->EditRotation(transIndex) = update.rotation;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::ANGULAR_VELOCITY))
		{
			_bodyStorage->EditRigidBodyAngularVelocity(bodyID) = update.angularVelocity;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::TORQUE))
		{
			_bodyStorage->EditRigidBodyTorque(bodyID) = update.torque;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::MASS))
		{
			_bodyStorage->SetRigidBodyMass(bodyID, update.mass);
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::IS_GRAVITY))
		{
			_bodyStorage->EditRigidBodyIsGravity(bodyID) = update.isGravity;
		}
		if (HasFlag(update.flags, RigidBodyUpdateFlag::GRAVITY))
		{
			_bodyStorage->EditRigidBodyGravity(bodyID) = update.gravity;
		}


	}
}
