#include "AnimationComponentAPI.h"

#include "RagdollComponent.h"

RagdollComponent::RagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const std::string& _path)
{
	id = AnimationComponentAPI::CreateRagdoll(_entity, rendererComponent, _path);
}
