#include "ServiceLocator.h"

#include "InputAction.h"

// 押された瞬間の関数登録
bool InputAction::AddStartedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return ServiceLocator::GetInputSytem()->QueueStartedCallbackAdd(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 条件達成時の関数登録
bool InputAction::AddPerformedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return ServiceLocator::GetInputSytem()->QueuePerformedCallbackAdd(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 離した瞬間の関数登録
bool InputAction::AddCanceledCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return ServiceLocator::GetInputSytem()->QueueCanceledCallbackAdd(InputSystem::CallbackAccessKey{}, id, _func, _user);
}

// 押された瞬間の関数登録解除
bool InputAction::RemoveStartedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return ServiceLocator::GetInputSytem()->StartedCallbackRemove(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 条件達成時の関数登録解除
bool InputAction::RemovePerformedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return ServiceLocator::GetInputSytem()->PerformedCallbackRemove(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 離した瞬間の関数登録解除
bool InputAction::RemoveCanceledCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return ServiceLocator::GetInputSytem()->CanceledCallbackRemove(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
