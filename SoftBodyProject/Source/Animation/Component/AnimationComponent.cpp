#include "AnimationComponentAPI.h"

#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(EntityID _entity, const RendererComponent* _renderer)
{
	id = AnimationComponentAPI::CreateAnimation(_entity, _renderer);
}