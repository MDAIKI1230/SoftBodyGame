#pragma once

#include "EntityID.h"
#include "CollisionInfo.h"

struct OnCollisionEnterEvent
{
	EntityID a, b;
	// Aに渡す情報
	CollisionInfo infoA;
	// Bに渡す情報
	CollisionInfo infoB;
};

struct OnCollisionEvent
{
	EntityID a, b;
	// Aに渡す情報
	CollisionInfo infoA;
	// Bに渡す情報
	CollisionInfo infoB;
};

struct OnCollisionExitEvent
{
	EntityID a, b;
	// Aに渡す情報
	CollisionInfo infoA;
	// Bに渡す情報
	CollisionInfo infoB;
};
