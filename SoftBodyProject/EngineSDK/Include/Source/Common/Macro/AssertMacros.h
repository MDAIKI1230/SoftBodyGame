#pragma once

#ifdef _DEBUG

#include <crtdbg.h>
#include <cstdlib>
#include <intrin.h>

namespace MD::Debug
{
	__declspec(noreturn) inline void ReportUnreachable(const char* _file, int _line, const char* _message)
	{
		_CrtDbgReport(_CRT_ASSERT, _file, _line, nullptr, "%s", _message);

		__debugbreak();
		std::abort();
	}
}

// 本来来ちゃいけないところまで来ちゃってるよ
#define MD_UNREACHABLE(message) \
	::MD::Debug::ReportUnreachable(__FILE__, __LINE__, message)

// デバッグ時だけログを出力する（printf形式）。
#define MD_DEBUG_LOG(...) \
	do \
	{ \
		_CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, nullptr, __VA_ARGS__); \
	} while (false)

// Releaseでは条件式とログ引数も評価しない。
#define MD_DEBUG_LOG_IF(condition, ...) \
	do \
	{ \
		if (condition) \
		{ \
			MD_DEBUG_LOG(__VA_ARGS__); \
		} \
	} while (false)

#else

#define MD_UNREACHABLE(message) __assume(0)

#define MD_DEBUG_LOG(...) ((void)0)
#define MD_DEBUG_LOG_IF(condition, ...) ((void)0)

#endif