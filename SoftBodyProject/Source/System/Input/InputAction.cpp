#include "InputSystem.h"

#include "InputAction.h"

// 押された瞬間の関数登録
bool InputAction::AddStartedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return InputSystem::QueueStartedCallbackAdd(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 条件達成時の関数登録
bool InputAction::AddPerformedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return InputSystem::QueuePerformedCallbackAdd(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 離した瞬間の関数登録
bool InputAction::AddCanceledCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return InputSystem::QueueCanceledCallbackAdd(InputSystem::CallbackAccessKey{}, id, _func, _user);
}

// 押された瞬間の関数登録解除
bool InputAction::RemoveStartedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return InputSystem::StartedCallbackRemove(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 条件達成時の関数登録解除
bool InputAction::RemovePerformedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return InputSystem::PerformedCallbackRemove(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
// 離した瞬間の関数登録解除
bool InputAction::RemoveCanceledCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
    return InputSystem::CanceledCallbackRemove(InputSystem::CallbackAccessKey{}, id, _func, _user);
}
