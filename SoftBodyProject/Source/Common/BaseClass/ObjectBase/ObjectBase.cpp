#include "TransformComponent.h"

#include "MonoBehaviour.h"

// コンストラクタ
ObjectBase::ObjectBase(WorldStorage* _world, EntityID _entityID) :
	world{ _world },
	id{ _entityID }
{
	AddComponent<TransformComponent>();
}