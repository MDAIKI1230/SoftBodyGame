#pragma once

#include "InputActionContext.h"
#include "InputAction.h"

#include "MonoBehaviour.h"

#include "Camera.h"

class TutorialBox :public MonoBehaviour
{
public:
	// コンストラクタ
	TutorialBox(WorldStorage* _world, EntityID _entityID);
};
