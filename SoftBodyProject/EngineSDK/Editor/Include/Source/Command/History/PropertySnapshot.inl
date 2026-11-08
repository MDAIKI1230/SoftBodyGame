#pragma once

#include "PropertySnapshot.h"

template<class T>
void PropertySnapshot::Capture(const T& _value)
{
	// コピーを作る
	T* ptr{ new T(_value) };

	saved = std::unique_ptr<void, Deleter>(
		ptr,
		// 破棄処理
		[](void* p)
		{
			delete static_cast<T*>(p);
		}
	);
	
	// 書き戻し処理
	writer =
		[](void* _target, const void* _saved)
		{
			*static_cast<T*>(_target) = *static_cast<const T*>(_saved);
		};

	// 同値チェック
	comparer =
		[](const void* _left, const void* _right)
		{
			return *static_cast<const T*>(_left) ==
				*static_cast<const T*>(_right);
		};

	valueType = &typeid(T);
}