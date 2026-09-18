#include "AnimationComponentAPI.h"

#include "ActiveRagdollComponent.h"

ActiveRagdollComponent::ActiveRagdollComponent(EntityID _entity, const RendererComponent& rendererComponent, const std::string& _path)
{
	id = AnimationComponentAPI::CreateActiveRagdoll(_entity, rendererComponent, _path);
}
