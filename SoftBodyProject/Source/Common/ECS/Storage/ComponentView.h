#pragma once

#include <cstddef>
#include <ranges>
#include <type_traits>

#include "EntityID.h"

// ComponentViewの条件をまとめてくれるやつ
template<class T>
struct ComponentViewGetter
{
    using StoragePointer =
        std::conditional_t<
        std::is_const_v<T>, // constやったらconstでポインタを取得するやで
        const void*,
        void*
        >;

    // 取得条件関数
    using Getter = T& (*)(StoragePointer,EntityID,std::size_t);

    StoragePointer storage{};
    EntityID entity{};
    Getter getter{};

    // ここが条件にあうデータを返してる。
    T& operator()(std::size_t index) const
    {
        return getter(storage, entity, index);
    }
};


template<class T>
using ComponentView =
std::ranges::transform_view<
    // ある点からある範囲分でViewGetterの条件で作られたViewの型
    std::ranges::iota_view<
    std::size_t,
    std::size_t
    >,
    ComponentViewGetter<T>
>;
