#pragma once

struct Color
{
public:
	// コンストラクタ
	Color() = default;
	// コンストラクタ
	Color(float _r, float _g, float _b) :
		r{ _r },
		g{ _g },
		b{ _b }
	{
	}
	// コンストラクタ
	Color(float _r, float _g, float _b, float _a) :
		r{ _r },
		g{ _g },
		b{ _b },
		a{ _a }
	{
	}
public:
	float r{ 0 }, g{ 0 }, b{ 0 }, a{ 1.0f };
};
