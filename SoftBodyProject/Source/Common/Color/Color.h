#pragma once

struct Color
{
public:
	// コンストラクタ
	Color() = default;
	// コンストラクタ
	Color(unsigned int _r, unsigned int _g, unsigned int _b) :
		r{ _r },
		g{ _g },
		b{ _b }
	{
	}
	// コンストラクタ
	Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a) :
		r{ _r },
		g{ _g },
		b{ _b },
		a{ _a }
	{
	}
public:
	unsigned int r{ 0 }, g{ 0 }, b{ 0 }, a{ 255 };
};