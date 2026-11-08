#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "MonoBehaviour.h"
#include "WorldStorage.h"

class ObjectFactory
{
	using CreateObjectFunc = std::unique_ptr<MonoBehaviour>(*)(WorldStorage*, EntityID);
public:
	static std::unordered_map<std::string, CreateObjectFunc> CreateFuncs;
private:
	template<class Object>
	static std::unique_ptr<MonoBehaviour> Create(WorldStorage* _world, EntityID _entity);

};

#include "ObjectFactory.inl"