#pragma 

#include "RendererComponent.h"

#include "AnimationID.h"

struct AnimationComponent
{
	friend class AnimationComponentStorage;
public:
	AnimationComponent(EntityID _entity, const RendererComponent* _renderer);
private:
	AnimationID id;
};