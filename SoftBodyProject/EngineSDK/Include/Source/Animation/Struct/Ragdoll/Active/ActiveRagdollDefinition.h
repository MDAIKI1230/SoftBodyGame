#pragma once

#include "RagdollDefinition.h"
#include "ActiveRagdollSetting.h"
#include "BoneMask.h"

struct ActiveRagdollDefinition
{
	RagdollDefinition ragdollDefinition;
	ActiveRagdollSetting settings;
	bool isRotationWeight{ false };
	BoneMask rotationWeights;
	bool isPositionWeight{ false };
	BoneMask positionWeights;
};