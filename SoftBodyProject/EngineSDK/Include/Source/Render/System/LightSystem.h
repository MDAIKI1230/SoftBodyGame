#pragma once

#include "MDMath.h"
#include "RenderingSystem.h"

struct TransformComponent;

class LightSystem : public RenderingSystem
{
public:
	LightSystem();
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;

private:
	static Vector3 GetWorldDirection(const ComponentStorageBase<TransformComponent>& _transforms, const TransformComponent& _transform);
};