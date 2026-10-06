#include "EventSystem.h"

void EventSystem::Update(EventManager* _eventManager, ObjectManager* _objectManager)
{
	// ゲームオブジェクトの変数(呼び出すため)
	MonoBehaviour* objectA;
	MonoBehaviour* objectB;
	// OnCollisionEnterEventの呼び出し
	OnCollisionEnterEvent onCollisionEnterEvent;
	while (_eventManager->Pop(onCollisionEnterEvent))
	{
		// 情報を埋めておく
		objectA = _objectManager->Get(onCollisionEnterEvent.a);
		objectB = _objectManager->Get(onCollisionEnterEvent.b);

		onCollisionEnterEvent.infoA.other = objectB;
		onCollisionEnterEvent.infoB.other = objectA;
		if (objectA != nullptr)
		{
			objectA->OnCollisionEnter(onCollisionEnterEvent.infoA);
		}
		
		if (objectB != nullptr)
		{
			objectB->OnCollisionEnter(onCollisionEnterEvent.infoB);
		}
	}
	// OnCollisionEventの呼び出し
	OnCollisionEvent onCollisionEvent;
	while (_eventManager->Pop(onCollisionEvent))
	{
		// 情報を埋めておく
		objectA = _objectManager->Get(onCollisionEvent.a);
		objectB = _objectManager->Get(onCollisionEvent.b);

		onCollisionEvent.infoA.other = objectB;
		onCollisionEvent.infoB.other = objectA;
		if (objectA != nullptr)
		{
			objectA->OnCollision(onCollisionEvent.infoA);
		}

		if (objectB != nullptr)
		{
			objectB->OnCollision(onCollisionEvent.infoB);
		}
	}
	// OnCollisionExitEventの呼び出し
	OnCollisionExitEvent onCollisionExitEvent;
	while (_eventManager->Pop(onCollisionExitEvent))
	{
		// 情報を埋めておく
		objectA = _objectManager->Get(onCollisionExitEvent.a);
		objectB = _objectManager->Get(onCollisionExitEvent.b);

		onCollisionExitEvent.infoA.other = objectB;
		onCollisionExitEvent.infoB.other = objectA;
		if (objectA != nullptr)
		{
			objectA->OnCollisionExit(onCollisionExitEvent.infoA);
		}

		if (objectB != nullptr)
		{
			objectB->OnCollisionExit(onCollisionExitEvent.infoB);
		}
	}
}
