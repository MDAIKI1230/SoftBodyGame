#include "AnimationComponentAPI.h"

#include "RagdollComponent.h"

RagdollComponent::RagdollComponent(EntityID _entity, const RendererComponent& rendererComponent)
{
	id = AnimationComponentAPI::CreateRagdoll(_entity, rendererComponent);
}
