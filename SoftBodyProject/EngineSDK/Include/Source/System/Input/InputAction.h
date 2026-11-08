#pragma once

#include <type_traits>

#include "Struct/InputActionRuntimeData.h"
#include "Struct/InputActionAssetData.h"

class InputAction
{
    friend class InputSystem;
public:
    // コンストラクタ
    InputAction() = default;

    // 押された瞬間の関数登録(複数回追加するとその分よばれるよ/解除も何回か呼ばないとダメ)
    bool AddStartedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
    // 条件達成時の関数登録(複数回追加するとその分よばれるよ/解除も何回か呼ばないとダメ)
    bool AddPerformedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
    // 離した瞬間の関数登録(複数回追加するとその分よばれるよ/解除も何回か呼ばないとダメ)
    bool AddCanceledCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user);

    // 押された瞬間の関数登録解除
    bool RemoveStartedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
    // 条件達成時の関数登録解除
    bool RemovePerformedCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
    // 離した瞬間の関数登録解除
    bool RemoveCanceledCallback(InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
    // メンバー関数登録
    template<auto Method, class T>
    bool AddStartedCallback(T* _instance)
    {
        if (_instance == nullptr)
        {
            return false;
        }

        return AddStartedCallback(
            &InvokeMember<Method, T>,
            _instance);
    }

    template<auto Method, class T>
    bool AddPerformedCallback(T* _instance)
    {
        if (_instance == nullptr)
        {
            return false;
        }

        return AddPerformedCallback(&InvokeMember<Method, T>, _instance);
    }

    template<auto Method, class T>
    bool AddCanceledCallback(T* _instance)
    {
        if (_instance == nullptr)
        {
            return false;
        }

        return AddCanceledCallback(&InvokeMember<Method, T>, _instance);
    }

    // メンバー関数解除
    template<auto Method, class T>
    bool RemoveStartedCallback(T* _instance)
    {
        return RemoveStartedCallback(&InvokeMember<Method, T>, _instance);
    }

    template<auto Method, class T>
    bool RemovePerformedCallback(T* _instance)
    {
        return RemovePerformedCallback(&InvokeMember<Method, T>, _instance);
    }

    template<auto Method, class T>
    bool RemoveCanceledCallback(T* _instance)
    {
        return RemoveCanceledCallback(&InvokeMember<Method, T>, _instance);
    }

private:
    InputAction(InputDataID _id)
        : id{ _id }
    {
    }

    // 関数を呼ぶための中間関数
    template<auto Method, class T>
    static void InvokeMember(
        void* _instance,
        InputActionContext& _context)
    {
        static_assert(std::is_member_function_pointer_v<decltype(Method)>);

        (static_cast<T*>(_instance)->*Method)(_context);
    }
private:
    InputDataID id{ 0 };
};
