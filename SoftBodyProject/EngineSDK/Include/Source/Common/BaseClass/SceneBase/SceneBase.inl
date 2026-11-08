#pragma once

#include "SceneBase.h"

template<class Object>
Object* SceneBase::Create()
{
	EntityID id{ objectManager.Allocate() };
	std::unique_ptr<Object> object{ std::make_unique<Object>(&worldStorage, id) };
	Object* ptr{ object.get() };
	objectManager.Add(std::move(object), id);
	return ptr;
}