#include "AnimationComponentAPI.h"

#include "RagdollComponent.h"

RagdollComponent::RagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const RagdollDefinition& _definition)
{
	id = AnimationComponentAPI::CreateRagdoll(_entity, rendererComponent, _definition);
}
