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
    void Push(const T& _event)
    {
        EventQueue<T>& queue{ GetQueue<T>() };

        queue.Push(_event);
    }

    // 取得
    template<typename T>
    bool Pop(T& _out)
    {
        EventQueue<T>& queue{ GetQueue<T>() };

        return queue.Pop(_out);
    }

    void Swap()
    {
#define X(type) \
            type##Queue.Swap(); \

            EVENT_LIST 
#undef X
    }
private:
    // キュー取得関数
    template<typename T>
    EventQueue<T>& GetQueue();

private:
    // キュー生成マクロ
#define X(type) EventQueue<type> type##Queue;
    EVENT_LIST
#undef X
};

// キュー関数生成マクロ
#define X(type) \
    template<> \
    inline EventQueue<type>&  EventManager::GetQueue<type>() \
    {\
        return type##Queue; \
    }

    EVENT_LIST
#undef X
