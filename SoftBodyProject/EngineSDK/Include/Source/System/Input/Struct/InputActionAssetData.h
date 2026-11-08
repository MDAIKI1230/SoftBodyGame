#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "KeyConstants.h"
#include "MouseConstants.h"
#include "GamePadConstants.h"

// 入力設定内の各要素を識別する永続ID
struct InputDataID
{
	// 0なら無効値
	std::uint64_t value{ 0 };

	[[nodiscard]]
	bool IsValid() const noexcept
	{
		return value != 0;
	}

	friend bool operator==(
		const InputDataID&,
		const InputDataID&) = default;
};

// 入力の扱い方
enum class InputActionType : std::uint8_t
{
	INVALID,
	BUTTON,
	VALUE,
	PASS_THROUGH
};

// Actionが最終的に公開する値の型
enum class InputValueType : std::uint8_t
{
	INVALID,
	BOOL,
	FLOAT,
	VECTOR2
};

// 発動タイミング
enum class InputInteractionType
{
	PRESS,
	HOLD,
	TAP
};

// 複数のPartを一つの値へ合成する方式
enum class InputCompositeType : std::uint8_t
{
	INVALID,
	AXIS2D
};

// Partの役割
enum class InputCompositePartRole : std::uint8_t
{
	INVALID,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// デバイスの種類
enum class InputDeviceType : std::uint8_t
{
	INVALID,
	KEYBOARD,
	MOUSE,
	GAMEPAD
};

// すべてのキー
struct AnyButtonControl
{
	InputDeviceType device{ InputDeviceType::INVALID };
};

// すべての入力系を一意に扱うための奴
using ResolvedInputControl =
std::variant<
	std::monostate,
	KeyConstants,
	MouseConstants,
	GamePadConstants,
	AnyButtonControl
>;

// 発動タイミング情報
struct InputInteractionData
{
	InputInteractionType type{ InputInteractionType::PRESS };

	// Holdに必要な時間
	float duration{ 0.0f };
};

// 入力デバイスの種類
struct InputControlSchemeData
{
	// ID
	InputDataID id;
	// 名前
	std::string name;
	// デバイスの入力の種類
	std::vector<InputDeviceType> device;
};

// 複数のPartから一つの値を合成する時のPart情報
struct InputBindingPartData
{
	// ID
	InputDataID id;
	// Partの役割
	InputCompositePartRole role{ InputCompositePartRole::INVALID };
	// 解決済みの入力Control
	ResolvedInputControl control;
};

// 単一ControlをActionへ直接結び付けるBinding固有情報
struct InputControlBindingData
{
	// 解決済みの入力Control
	ResolvedInputControl control;
};

// 複数のControlを一つの値へ合成するBinding固有情報
struct InputCompositeBindingData
{
	// Partの合成方式
	InputCompositeType type{ InputCompositeType::INVALID };

	// 複数のPart
	std::vector<InputBindingPartData> parts;
};

// Kindから2つを使い分ける用。
using InputBindingSourceData =
std::variant<
	std::monostate,
	InputControlBindingData,
	InputCompositeBindingData
>;

// 入力バインド情報
struct InputBindingData
{
	// バインドID
	InputDataID id;
	// 名前
	std::string name;

	// 入力デバイスID
	std::vector<InputDataID> schemeIds;
	// 入力を作るためのバインド情報
	InputBindingSourceData source;

	// デッドゾーン
	float deadZone{ 0.0f };

};

// 入力アクション情報
struct InputActionData
{
	// ID
	InputDataID id;
	// 名前
	std::string name;

	// 入力の扱い方
	InputActionType type{ InputActionType::INVALID };

	// 最終的な入力値の設定
	InputValueType valueType{ InputValueType::INVALID };

	// 発動タイミング情報
	InputInteractionData interaction;

	// 入力バインド情報
	std::vector<InputBindingData> bindings;
};

// 入力アクションマップ情報
struct InputActionMapData
{
	// ID
	InputDataID id;
	// 名前
	std::string name;

	// 入力アクション情報
	std::vector<InputActionData> actions;
};

// アセット全体の情報
struct InputActionAssetData
{
	// バージョン
	std::uint32_t version{ 1 };

	// ID
	InputDataID id;
	// 名前
	std::string name;

	// 利用するデバイス構成の定義
	std::vector<InputControlSchemeData> controlSchemes;
	// 入力アクションマップ
	std::vector<InputActionMapData> actionMaps;
};
