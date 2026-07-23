#include "TransformComponent.h"

#include "EmptyObject.h"

EmptyObject::EmptyObject(WorldStorage* world, const EntityID& _entity) :
	ObjectBase{ world,_entity }
{
	AddComponent<TransformComponent>();
}

void EmptyObject::Update()
{

}

void EmptyObject::FixedUpdate()
{

}

void EmptyObject::OnCollisionEnter()
{

}

void EmptyObject::OnCollision()
{

}

void EmptyObject::OnCollisionExit()
{

}
