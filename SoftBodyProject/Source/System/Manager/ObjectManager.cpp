#include "ObjectManager.h"


void ObjectManager::Add(ObjectBase* _object)
{
	objects.push_back(_object);
}

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

ObjectBase* ObjectManager::Get(int _index)
{
	return objects[_index];
}