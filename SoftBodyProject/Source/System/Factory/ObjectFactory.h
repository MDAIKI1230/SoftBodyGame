#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "WorldStorage.h"

#include "ObjectBase.h"

class ObjectFactory
{
	using CreateObjectFunc = std::unique_ptr<ObjectBase>(*)(WorldStorage*, EntityID);
public:
	static std::unique_ptr<ObjectBase> CreateDebugBox(WorldStorage* world, EntityID _entity);
	static std::unique_ptr<ObjectBase> CreateSphereBox(WorldStorage* world, EntityID _entity);
public:
	static std::unordered_map<std::string, CreateObjectFunc> CreateFuncs;
};
