#pragma once

#include "ObjectFactory.h"

template<class Object>
static std::unique_ptr<MonoBehaviour> ObjectFactory::Create(WorldStorage* _world, EntityID _entity)
{
	return std::make_unique<Object>(_world, _entity);
}