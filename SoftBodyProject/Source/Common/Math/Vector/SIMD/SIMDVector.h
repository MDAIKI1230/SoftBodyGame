#pragma once

#include <xmmintrin.h>
#include <immintrin.h>

// 128bit SIMDレジスタラッパー構造体なので、ヘッダ内で収める

struct alignas(16) SIMDVectorFloat
{
public:
	__m128 value;
public:
	// コンストラクタ
	SIMDVectorFloat():
		value{ _mm_setzero_ps()}
	{}

	/// <param name="_value"></param>
	SIMDVectorFloat(__m128 _value):
		value{ _value }
	{}

	/// <param name="_ptr">最初の値のアドレス</param>
	SIMDVectorFloat(const float* _ptr) :
		value{ _mm_load_ps(_ptr) }
	{}

	/// <param name="_x">値1</param>
	/// <param name="_y">値2</param>
	/// <param name="_z">値3</param>
	/// <param name="_w">値4</param>
	SIMDVectorFloat(float _x, float _y, float _z, float _w) :
		value{ _mm_set_ps(_w,_z,_y,_x) }
	{}

	// splat
	SIMDVectorFloat(float v):
		value{ _mm_set1_ps(v) }
	{}

	// 型変換演算子
	operator __m128() const
	{
		return value;
	}

	/// <summary>
	/// 値引き出し関数
	/// </summary>
	/// <param name="_ptr">最初の値のアドレス</param>
	void Store(float* _ptr)const
	{
		_mm_store_ps(_ptr, value);
	}

	/// <summary>
	/// 値入れ替え
	/// </summary>
	/// <typeparam name="X">値1がどこと変わるか</typeparam>
	/// <typeparam name="Y">値1がどこと変わるか</typeparam>
	/// <typeparam name="Z">値1がどこと変わるか</typeparam>
	/// <typeparam name="W">値1がどこと変わるか</typeparam>
	/// <param name="_value">入れ替える元の値</param>
	/// <returns>入れ替えられた新しい値</returns>
	template<int X,int Y,int Z,int W>
	static SIMDVectorFloat Shuffle(const SIMDVectorFloat& _value)
	{
		return _mm_permute_ps(_value, _MM_SHUFFLE(W, Z, Y, X));
	}

	/// <summary>
	/// 値入れ替え
	/// </summary>
	/// <typeparam name="X">値1がどこと変わるか</typeparam>
	/// <typeparam name="Y">値1がどこと変わるか</typeparam>
	/// <typeparam name="Z">値1がどこと変わるか</typeparam>
	/// <typeparam name="W">値1がどこと変わるか</typeparam>
	/// <param name="_value">入れ替える元の値</param>
	/// <returns>入れ替えられた新しい値</returns>
	template<int X, int Y, int Z, int W>
	static SIMDVectorFloat Shuffle(const SIMDVectorFloat& _value1, const SIMDVectorFloat _value2)
	{
		return _mm_shuffle_ps(_value1, _value2, _MM_SHUFFLE(W, Z, Y, X));
	}


	// 交互に並べる(上の2つ)
	static SIMDVectorFloat UnpackHigh(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
	{
		return _mm_unpackhi_ps(_vec1, _vec2);
	}

	// 交互に並べる(上の2つ)
	static SIMDVectorFloat UnpackLow(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
	{
		return _mm_unpacklo_ps(_vec1, _vec2);
	}

	// 上二つを並べる
	static SIMDVectorFloat MoveHigh(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
	{
		return _mm_movehl_ps(_vec2, _vec1);
	}

	// 下二つを並べる
	static SIMDVectorFloat MoveLow(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
	{
		return _mm_movelh_ps(_vec1, _vec2);
	}

	// Blend関数(マスクの意味　0 = 第一引数 1 = 第二引数
	template<int mask>
	static SIMDVectorFloat Blend(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
	{
		return _mm_blend_ps(_vec1, _vec2, mask);
	}
};

struct alignas(16) SIMDVectorInt
{
public:
	__m128i value;
public:
	// コンストラクタ
	SIMDVectorInt() :
		value{ _mm_setzero_si128() }
	{
	}

	/// <param name="_value"></param>
	SIMDVectorInt(__m128i _value) :
		value{ _value }
	{
	}

	/// <param name="_ptr">最初の値のアドレス</param>
	SIMDVectorInt(const int* _ptr) :
		value{ _mm_load_si128((__m128i*)_ptr) }
	{
	}

	/// <param name="_x">値1</param>
	/// <param name="_y">値2</param>
	/// <param name="_z">値3</param>
	/// <param name="_w">値4</param>
	SIMDVectorInt(int _x, int _y, int _z, int _w) :
		value{ _mm_set_epi32(_w,_z,_y,_x) }
	{
	}

	// splat
	SIMDVectorInt(int v) :
		value{ _mm_set1_epi32(v) }
	{
	}

	// 型変換演算子
	operator __m128i() const
	{
		return value;
	}

	/// <summary>
	/// 値引き出し関数
	/// </summary>
	/// <param name="_ptr">最初の値のアドレス</param>
	void Store(int* _ptr)const
	{
		_mm_store_si128((__m128i*)_ptr, value);
	}

	/// <summary>
	/// 値入れ替え
	/// </summary>
	/// <typeparam name="X">値1がどこと変わるか</typeparam>
	/// <typeparam name="Y">値1がどこと変わるか</typeparam>
	/// <typeparam name="Z">値1がどこと変わるか</typeparam>
	/// <typeparam name="W">値1がどこと変わるか</typeparam>
	/// <param name="_value">入れ替える元の値</param>
	/// <returns>入れ替えられた新しい値</returns>
	template<int X, int Y, int Z, int W>
	static SIMDVectorInt Shuffle(const SIMDVectorInt& _value)
	{
		return _mm_shuffle_epi32(_value, _value, _MM_SHUFFLE(W, Z, Y, X));
	}
};