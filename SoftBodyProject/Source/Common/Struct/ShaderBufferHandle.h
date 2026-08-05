#pragma once

#include "GenerationalID.h"

struct ShaderBufferTag;

class ShaderBufferHandle :public GenerationalID<ShaderBufferTag>
{
public:
	ShaderBufferHandle() = default;
	ShaderBufferHandle(Index _index, Generation _generation) :
		GenerationalID(_index, _generation)
	{
	}

	ShaderBufferHandle(Index _index, Generation _generation, uint32_t _size) :
		GenerationalID(_index, _generation),
		size{ _size }
	{
	}

	uint32_t GetSize()
	{
		return size;
	}

private:
	uint32_t size{ 0 };
};

