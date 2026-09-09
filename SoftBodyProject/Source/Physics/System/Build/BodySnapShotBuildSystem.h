#pragma once

#include "BodySnapshotBuffer.h"
#include "PhysicsTransformStorage.h"
#include "BodyStorage.h"

class BodySnapshotBuildSystem
{
public:
	void Build(BodySnapshotBuffer* _bodySnapshotBuffer, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage);
};
