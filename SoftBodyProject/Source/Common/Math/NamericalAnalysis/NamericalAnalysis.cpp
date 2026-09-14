#include <algorithm>

#include "NamericalAnalysis.h"

Vector4 NamericalAnalysis::GaussJordan(const Matrix4x4& _coefficientMatrix, const Vector4& _vec, Matrix4x4* inverseMatrix)
{
	Matrix4x4 mat{ _coefficientMatrix };
	Vector4 vec{ _vec };
	// 逆行列が必要そうなら単位行列で初期化
	if (inverseMatrix)
	{
		*inverseMatrix = Matrix4x4::IDENTITY;
	}

	for (int k{ 0 }; k < 4; k++)
	{
		int pivot{ k };
		float pivotValue{ 0.0f };

		// ピボット選択
		for (int i{ k }; i < 4; i++)
		{
			// 選択する所から順に行を見ていって絶対値が一番大きいものを選ぶ
			float abs{ std::abs(mat.m[i][k]) };
			if (abs > pivotValue)
			{
				pivot = i;
				pivotValue = abs;
			}
		}

		// 並び変え(必要なら)
		if (pivot != k)
		{
			// 一行並び変え
			SIMDVectorFloat tempVec{ mat.row[k] };
			mat.row[k] = mat.row[pivot];
			mat.row[pivot] = tempVec;

			// 定数項ベクトルも並び変え
			float temp{ vec.elements[k] };
			vec.elements[k] = vec.elements[pivot];
			vec.elements[pivot] = temp;

			// 逆行列が必要なら並び変え
			if (inverseMatrix)
			{
				tempVec = inverseMatrix->row[k];
				inverseMatrix->row[k] = inverseMatrix->row[pivot];
				inverseMatrix->row[pivot] = tempVec;
			}
		}
		// 逆行列なし、または解けない
		if (pivotValue < MathConstants::EPSILON)
		{
			return Vector4::ZERO;
		}

		// 前進消去
		// 対角成分を1にしていく。
		pivotValue = mat.m[k][k];
		mat.row[k] = SIMDVectorMath::DivScalar(mat.row[k], pivotValue);
		vec.elements[k] /= pivotValue;

		// 逆行列が必要なら計算
		if (inverseMatrix)
		{
			inverseMatrix->row[k] = SIMDVectorMath::DivScalar(inverseMatrix->row[k], pivotValue);
		}

		for (int i{ 0 }; i < 4; i++)
		{
			if (i == k)
			{
				continue;
			}
			float factor = mat.m[i][k];
			// 定数項ベクトルの計算
			vec.elements[i] -= vec.elements[k] * factor;

			// 係数行列計算
			for (int j{ k }; j < 4; j++)
			{
				mat.m[i][j] -= mat.m[k][j] * factor;
			}
			// 逆行列が必要なら計算
			if (inverseMatrix)
			{
				for (int j{ 0 }; j < 4; j++)
				{
					inverseMatrix->m[i][j] -= inverseMatrix->m[k][j] * factor;
				}
			}
		}
	}

	return vec;
}

Matrix4x4 NamericalAnalysis::GaussJordan(const Matrix4x4& _coefficientMatrix)
{
	Matrix4x4 mat{ _coefficientMatrix };
	Matrix4x4 result{ Matrix4x4::IDENTITY };

	for (int k{ 0 }; k < 4; k++)
	{
		int pivot{ k };
		float pivotValue{ 0.0f };

		// ピボット選択
		for (int i{ k }; i < 4; i++)
		{
			// 選択する所から順に行を見ていって絶対値が一番大きいものを選ぶ
			float abs{ std::abs(mat.m[i][k]) };
			if (abs > pivotValue)
			{
				pivot = i;
				pivotValue = abs;
			}
		}

		// 並び変え(必要なら)
		if (pivot != k)
		{
			// 一行並び変え
			SIMDVectorFloat tempVec{ mat.row[k] };
			mat.row[k] = mat.row[pivot];
			mat.row[pivot] = tempVec;

			// 逆行列が必要なら並び変え
			tempVec = result.row[k];
			result.row[k] = result.row[pivot];
			result.row[pivot] = tempVec;
		}
		// 逆行列なし、または解けない
		if (pivotValue < MathConstants::EPSILON)
		{
			return Matrix4x4::ZERO;
		}

		// 前進消去
		// 対角成分を1にしていく。
		pivotValue = mat.m[k][k];
		mat.row[k] = SIMDVectorMath::DivScalar(mat.row[k], pivotValue);

		result.row[k] = SIMDVectorMath::DivScalar(result.row[k], pivotValue);

		for (int i{ 0 }; i < 4; i++)
		{
			if (i == k)
			{
				continue;
			}
			float factor = mat.m[i][k];

			// 係数行列計算
			for (int j{ k }; j < 4; j++)
			{
				mat.m[i][j] -= mat.m[k][j] * factor;
			}
			// 逆行列が必要なら計算
			for (int j{ 0 }; j < 4; j++)
			{
				result.m[i][j] -= result.m[k][j] * factor;
			}
		}
	}

	return result;
}
