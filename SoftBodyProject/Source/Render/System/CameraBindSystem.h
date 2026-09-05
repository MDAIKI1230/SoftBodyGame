#pragma once

#include "LateUpdateSystem.h"

class CameraBindSystem :public LateUpdateSystem
{
public:
	CameraBindSystem()
	{
		priority = -100;
	}

	void LateUpdate(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};
