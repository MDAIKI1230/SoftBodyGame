#pragma once

#include "RagdollID.h"
#include "RendererComponent.h"

struct RagdollComponent
{
	friend class RagdollComponentStorage;
public:
	RagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const RagdollDefinition& _definition);
private:
	RagdollID id;
};
