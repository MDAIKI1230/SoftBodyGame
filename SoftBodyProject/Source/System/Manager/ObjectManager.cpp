#include "ObjectManager.h"


void ObjectManager::Add(std::unique_ptr<ObjectBase> _object)
{
	objects.push_back(std::move(_object));
}

void ObjectManager::Update()
{
	for (auto& obj : objects)
	{
		obj->Update();
	}
}

void ObjectManager::FixedUpdate()
{
	for (auto& obj : objects)
	{
		obj->FixedUpdate();
	}
}

ObjectBase* ObjectManager::Get(int _index)
{
	return objects[_index].get();
}