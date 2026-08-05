#pragma once

#include "BodyStorage.h"

#include "ComputeShaderHandle.h"
#include "ShaderBufferHandle.h"

class SoftSolverSystem
{
public:
	SoftSolverSystem() = default;
	void Initialize();
	void Solve(BodyStorage* _bodyStorage);
	~SoftSolverSystem();
private:
	ComputeShaderHandle computeShader;
	ShaderBufferHandle particleBuffer;
	uint32_t groupCount;
};
