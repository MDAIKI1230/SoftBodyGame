#pragma once

#include <cstdint>
#include <vector>

#include "RigidBodySnapshot.h"

struct RigidBodySnapshotFrame
{
    uint64_t completedTick;
    std::vector<RigidBodySnapshot> slots;
};
