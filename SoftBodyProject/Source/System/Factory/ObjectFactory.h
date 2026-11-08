#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "WorldStorage.h"

#include "MonoBehaviour.h"

class ObjectFactory
{
	using CreateObjectFunc = std::unique_ptr<MonoBehaviour>(*)(WorldStorage*, EntityID);
public:
	static std::unique_ptr<MonoBehaviour> CreateEmptyObject(WorldStorage* world, EntityID _entity);
	static std::unique_ptr<MonoBehaviour> CreateDebugBox(WorldStorage* world, EntityID _entity);
	static std::unique_ptr<MonoBehaviour> CreateSphereBox(WorldStorage* world, EntityID _entity);
public:
	static std::unordered_map<std::string, CreateObjectFunc> CreateFuncs;
};
