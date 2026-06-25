#include "EventSystem.h"

void EventSystem::Update(EventManager* _eventManager, ObjectManager* _objectManager)
{
	// OnCollisionEnterEventの呼び出し
	OnCollisionEnterEvent onCollisionEnterEvent;
	while (_eventManager->Pop(onCollisionEnterEvent))
	{
		_objectManager->Get(onCollisionEnterEvent.a)->OnCollisionEnter();
		_objectManager->Get(onCollisionEnterEvent.b)->OnCollisionEnter();
	}
	// OnCollisionEventの呼び出し
	OnCollisionEvent onCollisionEvent;
	while (_eventManager->Pop(onCollisionEvent))
	{
		_objectManager->Get(onCollisionEvent.a)->OnCollisionEnter();
		_objectManager->Get(onCollisionEvent.b)->OnCollisionEnter();
	}
	// OnCollisionExitEventの呼び出し
	OnCollisionExitEvent onCollisionExitEvent;
	while (_eventManager->Pop(onCollisionExitEvent))
	{
		_objectManager->Get(onCollisionExitEvent.a)->OnCollisionEnter();
		_objectManager->Get(onCollisionExitEvent.b)->OnCollisionEnter();
	}
}
