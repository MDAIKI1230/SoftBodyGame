#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "WorldStorage.h"

#include "ObjectBase.h"

class ObjectFactory
{
	using CreateObjectFunc = std::unique_ptr<ObjectBase>(*)(WorldStorage*, uint32_t);
public:
	static std::unique_ptr<ObjectBase> CreateDebugBox(WorldStorage* world, uint32_t _entity);
	static std::unique_ptr<ObjectBase> CreateSphereBox(WorldStorage* world, uint32_t _entity);
public:
	static std::unordered_map<std::string, CreateObjectFunc> CreateFuncs;
};
