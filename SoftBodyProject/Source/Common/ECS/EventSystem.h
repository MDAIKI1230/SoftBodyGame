#pragma once

#include "EventManager.h"
#include "ObjectManager.h"

class EventSystem
{
public:
	void Update(EventManager* _eventManager, ObjectManager* _objectManager);
};
