#pragma once

#include "EntityID.h"
#include "RagdollID.h"

#include "RendererComponent.h"

struct RagdollComponent
{
	friend class RagdollComponentStorage;
public:
	RagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const std::string& _path);
private:
	RagdollID id;
};
