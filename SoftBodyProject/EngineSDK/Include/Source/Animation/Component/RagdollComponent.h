#pragma once

#include "EntityID.h"
#include "RagdollID.h"

#include "ModelRendererComponent.h"

struct RagdollComponent
{
	friend class RagdollComponentStorage;
public:
	RagdollComponent(EntityID _entity, const ModelRendererComponent& rendererComponent, const std::string& _path);
private:
	RagdollID id;
};
