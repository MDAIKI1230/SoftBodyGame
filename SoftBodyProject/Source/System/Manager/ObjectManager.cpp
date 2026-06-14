#include "ObjectManager.h"

void ObjectManager::Update()
{
	for (auto obj : objects)
	{
		obj->Update();
	}
}

void ObjectManager::FixedUpdate()
{
	for (auto obj : objects)
	{
		obj->FixedUpdate();
	}
}