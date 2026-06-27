#pragma once

#include "EntityID.h"

struct OnCollisionEnterEvent
{
	EntityID a, b;
};

struct OnCollisionEvent
{
	EntityID a, b;
};

struct OnCollisionExitEvent
{
	EntityID a, b;
};
