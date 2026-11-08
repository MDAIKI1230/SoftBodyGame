#pragma once

#include <cstdint>

template<class HANDLE>
struct ResourceRow
{
	HANDLE handle;
	std::uint32_t ownerCount{ 0 };
};