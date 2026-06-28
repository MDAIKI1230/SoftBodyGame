#pragma once

#include "MDMath.h"

namespace NamericalAnalysis
{
	static Vector4 GaussJordan(const Matrix4x4& _coefficientMatrix,const Vector4& _vec, Matrix4x4* inverseMatrix = nullptr);
}
