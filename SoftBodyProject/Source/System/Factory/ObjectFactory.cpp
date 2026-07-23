#include "DebugBox.h"
#include "DebugSphere.h"

#include "ObjectFactory.h"

std::unique_ptr<ObjectBase> ObjectFactory::CreateDebugBox(WorldStorage* world, EntityID _entity)
{
	return std::make_unique<DebugBox>(world, _entity);
}

std::unique_ptr<ObjectBase> ObjectFactory::CreateSphereBox(WorldStorage* world, EntityID _entity)
{
	return std::make_unique<DebugSphere>(world, _entity);
}

std::unordered_map<std::string, ObjectFactory::CreateObjectFunc> ObjectFactory::CreateFuncs
{
	{ "DebugBox", &ObjectFactory::CreateDebugBox },
	{ "DebugSphere", &ObjectFactory::CreateSphereBox },
};
