#pragma once

#include "PhysicsEvent.h"
#include "EventQueue.h"

#define EVENT_LIST \
    X(OnCollisionEnterEvent) \
    X(OnCollisionEvent) \
    X(OnCollisionExitEvent)

class EventManager
{
public:
    // 追加
    template<typename T>
    T Push(const T& _event)
    {
        EventQueue<T>& queue{ GetQueue<T>() };

        return queue.Push();
    }

    // 取得
    template<typename T>
    T Pop(const T& _event)
    {
        EventQueue<T>& queue{ GetQueue<T>() };

        return queue.Pop();
    }

    void Swap()
    {
#define X(type) \
            type##Queue.Swap(); \

            EVENT_LIST 
#undef X
    }
private:
    // キュー関数生成マクロ
#define X(type) \
    template<> \
    inline EventQueue<type>&  GetQueue<type>() \
    {\
        return type##Queue; \
    }\
    EVENT_LIST
#undef X

private:
    // キュー生成マクロ
#define X(type) EventQueue<type> type##Queue;
    EVENT_LIST
#undef X
};
