#pragma once

#include <memory> 
#include <typeinfo>

class PropertySnapshot
{
public:
	// 値の保存
	template<class T>
	void Capture(const T& _value);
	// 保存した値を指定先へ書き戻す
	void ApplyTo(void* _target) const;
	// 無効値チェック
	bool IsValid() const;
	// 同値チェック
	bool operator==(const PropertySnapshot& _other) const;
	// 非同値チェック
	bool operator!= (const PropertySnapshot & _other) const;
private:
	using Deleter = void (*)(void*);
	using Writer = void (*)(void* _target, const void* _saved);
	using Comparer = bool (*)(const void*, const void*);

	// 実際の値
	std::unique_ptr<void, Deleter> saved{
		nullptr,
		[](void*) {}
	};

	Writer writer{ nullptr };
	Comparer comparer{ nullptr };
	const std::type_info* valueType{ nullptr };
};

#include "PropertySnapshot.inl"