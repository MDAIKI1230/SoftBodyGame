#pragma once

#include <string>

#include "EntityID.h"
#include "ActiveRagdollID.h"

#include "RendererComponent.h"

struct ActiveRagdollComponent
{
	friend class ActiveRagdollComponentStorage;
public:
	ActiveRagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const std::string& _path);
private:
	ActiveRagdollID id;
};