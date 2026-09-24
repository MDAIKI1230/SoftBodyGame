#pragma once

#include "InputActionContext.h"
#include "InputAction.h"

#include "ObjectBase.h"

#include "Camera.h"

class TutorialBox :public ObjectBase
{
public:
	// コンストラクタ
	TutorialBox(WorldStorage* _world, EntityID _entityID);
};
