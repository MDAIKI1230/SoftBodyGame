#pragma once

#include "MDMath.h"
#include "Struct/InputValue.h"

class InputActionContext
{
public:
	// Startedからの経過時間
	[[nodiscard]]
	float GetDuration() const noexcept
	{
		return duration;
	}

	template<class T>
	[[nodiscard]]
	T ReadValue() const
	{
		// 中身が何か調べてそれを返す
		if (const auto* value = std::get_if<T>(&currentValue))
		{
			return *value;
		}

		return {};
	}

	template<class T>
	[[nodiscard]]
	T ReadPreviousValue() const
	{
		// 中身が何か調べてそれを返す
		if (const auto* value = std::get_if<T>(&previousValue))
		{
			return *value;
		}

		return {};
	}

private:
	friend class InputSystem;

	// 外部へ直接公開しない
	InputValue currentValue;
	InputValue previousValue;

	float duration{ 0.0f };
};
