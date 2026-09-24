#include "TransformComponent.h"

#include "ObjectBase.h"

// コンストラクタ
ObjectBase::ObjectBase(WorldStorage* _world, EntityID _entityID) :
	world{ _world },
	id{ _entityID }
{
	AddComponent<TransformComponent>();
}