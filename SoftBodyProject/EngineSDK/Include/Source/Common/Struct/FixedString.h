#pragma once

/*
	テンプレート引数に文字列を取得したいときに使う
*/
template<size_t N>
struct FixedString
{
public:
	constexpr FixedString(const char(&str)[N])
	{
		for (size_t i = 0; i < N; ++i)
			value[i] = str[i];
	}

	constexpr const char* ToString() const noexcept
	{
		return value;
	}
public:
	char value[N];
};