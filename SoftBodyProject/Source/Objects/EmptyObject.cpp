#include "TransformComponent.h"

#include "EmptyObject.h"

EmptyObject::EmptyObject(WorldStorage* world, EntityID _entity) :
	MonoBehaviour{ world,_entity }
{
	AddComponent<TransformComponent>();
}

void EmptyObject::Update()
{

}

void EmptyObject::FixedUpdate()
{

}

void EmptyObject::OnCollisionEnter(CollisionInfo _info)
{

}

void EmptyObject::OnCollision(CollisionInfo _info)
{

}

void EmptyObject::OnCollisionExit(CollisionInfo _info)
{

}
