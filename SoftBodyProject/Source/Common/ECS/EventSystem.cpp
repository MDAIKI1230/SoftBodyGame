#include "EventSystem.h"

void EventSystem::Update(EventManager* _eventManager, ObjectManager* _objectManager)
{
	// ゲームオブジェクトの変数(呼び出すため)
	ObjectBase* object;
	// OnCollisionEnterEventの呼び出し
	OnCollisionEnterEvent onCollisionEnterEvent;
	while (_eventManager->Pop(onCollisionEnterEvent))
	{
		object = _objectManager->Get(onCollisionEnterEvent.a);
		if (object != nullptr)
		{
			object->OnCollisionEnter();
		}
		object = _objectManager->Get(onCollisionEnterEvent.b);
		if (object != nullptr)
		{
			object->OnCollisionEnter();
		}
	}
	// OnCollisionEventの呼び出し
	OnCollisionEvent onCollisionEvent;
	while (_eventManager->Pop(onCollisionEvent))
	{
		object = _objectManager->Get(onCollisionEvent.a);
		if (object != nullptr)
		{
			object->OnCollision();
		}
		object = _objectManager->Get(onCollisionEvent.b);
		if (object != nullptr)
		{
			object->OnCollision();
		}
	}
	// OnCollisionExitEventの呼び出し
	OnCollisionExitEvent onCollisionExitEvent;
	while (_eventManager->Pop(onCollisionExitEvent))
	{
		object = _objectManager->Get(onCollisionExitEvent.a);
		if (object != nullptr)
		{
			object->OnCollisionExit();
		}
		object = _objectManager->Get(onCollisionExitEvent.b);
		if (object != nullptr)
		{
			object->OnCollisionExit();
		}
	}
}
