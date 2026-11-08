#pragma once

#include <cstdint>
#include <string>

/*
	変更があった変数がどこかを保持する。
	オブジェクトIDが無効値じゃなくて、コンポーネントIDが無効値の場合オブジェクトの名前の変更として処理される
	0が無効値
*/
struct PropertyTarget
{
public:
	bool operator==(const PropertyTarget&) const = default;

public:
	std::uint64_t objectID{ 0 };
	std::uint64_t componentID{ 0 };
	std::string fieldName;
};