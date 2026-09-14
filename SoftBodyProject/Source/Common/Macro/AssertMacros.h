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

#else

#define MD_UNREACHABLE(message) __assume(0)

#endif