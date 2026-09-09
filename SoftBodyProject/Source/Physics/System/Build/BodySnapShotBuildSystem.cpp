#include "BodySnapshotBuildSystem.h"

void BodySnapshotBuildSystem::Build(BodySnapshotBuffer* _bodySnapshotBuffer, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
    auto& frame = _bodySnapshotBuffer->BeginWrite(_bodyStorage->CountRigidBodyID());

    for (BodyID bodyID : _bodyStorage->GetRigidBodyIDRange())
    {
        auto& output = _bodySnapshotBuffer->Edit(bodyID);
        PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
        uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };

        output.id = bodyID;
        output.previousPosition = _bodyStorage->GetRigidBodyPastPosition(bodyID);
        output.previousRotation = _bodyStorage->GetRigidBodyPastRotation(bodyID);
        output.currentPosition = _transformStorage->GetPosition(transIndex);
        output.currentRotation = _transformStorage->GetRotation(transIndex);
        output.velocity = _bodyStorage->GetRigidBodyVelocity(bodyID);
        output.angularVelocity = _bodyStorage->GetRigidBodyVelocity(bodyID);

        output.writtenTick = frame.completedTick;
    }

    _bodySnapshotBuffer->Publish();
}
