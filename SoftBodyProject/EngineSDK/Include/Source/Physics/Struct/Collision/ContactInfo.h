#pragma once

#include "MDMath.h"

/*
	法線
*/
struct  ContactInfo
{
public:
	ContactInfo() :
		contactLocalPositions{},
		depths{ 0.0f,0.0f,0.0f,0.0f }
	{
	}

public:
	// 衝突時のコライダー法線
	Vector3 normal;
	union  
	{
		// 衝突ローカル位置すべて
		Vector3 contactLocalPositions[4];
		// 衝突ローカル位置(先頭要素のみ)
		Vector3 contactLocalPosition;
	};
	union 
	{
		// 重なり深さすべて
		float depths[4];
		// 重なり深さ(先頭要素のみ)
		float depth;
	};
	

	// 衝突点の数
	uint32_t contactCount{ 0 };
};