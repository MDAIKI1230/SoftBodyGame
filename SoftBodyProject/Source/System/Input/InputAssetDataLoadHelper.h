#pragma once

#include <document.h>
#include <vector>
#include <string_view>
#include <optional>

#include "Struct/InputActionAssetData.h"

namespace InputAssetDataLoadHelper
{
	// 入力デバイスの種類取得関数
	bool GetControlSchemes(const rapidjson::Value& _value, std::vector<InputControlSchemeData>& _output);

	// 入力アクションマップ情報取得関数
	bool GetActionMaps(const rapidjson::Value& _value, std::vector<InputActionMapData>& _output);
	// 入力アクション情報取得関数
	bool GetInputActions(const rapidjson::Value& _value, std::vector<InputActionData>& _output);
	// 発動タイミング情報取得関数
	bool GetInteraction(const rapidjson::Value& _value, InputInteractionData& _output);
	// 入力バインド情報取得関数
	bool GetInputBinding(const rapidjson::Value& _value, std::vector<InputBindingData>& _output);
	// Binding固有情報取得関数
	bool GetBindingSource(const rapidjson::Value& _value, InputBindingSourceData& _output);
	// Control版(単一)Binding固有情報取得関数
	bool GetControlBinding(const rapidjson::Value& _value, InputBindingSourceData& _output);
	// Composite版(複数)Binding固有情報取得関数
	bool GetCompositeBinding(const rapidjson::Value& _value, InputBindingSourceData& _output);
	// Part情報取得関数
	bool GetBindingPart(const rapidjson::Value& _value, InputBindingPartData& _output);
	// Parts情報取得関数
	bool GetBindingParts(const rapidjson::Value& _value, std::vector<InputBindingPartData>& _output);
	// 発動タイミング種類をJsonから取ってくる
	bool GetInputInteractionType(const rapidjson::Value& _value, InputInteractionType& _output);
	// InputActionTypeをJsonから取ってくる
	bool GetInputActionType(const rapidjson::Value& _value, InputActionType& _output);
	// InputValueTypeをJsonから取ってくる
	bool GetInputValueType(const rapidjson::Value& _value, InputValueType& _output);
	// InputCompositeTypeをJsonから取ってくる
	bool GetInputCompositeType(const rapidjson::Value& _value, InputCompositeType& _output);
	// ResolvedInputControlをJsonから取ってくる
	bool GetResolvedInputControl(const rapidjson::Value& _value, ResolvedInputControl& _output);
	// パーツの役割をJsonから取ってくる
	bool GetInputCompositePartRole(const rapidjson::Value& _value, InputCompositePartRole& _output);
	// デバイス種類をJsonから取ってくる
	bool GetInputDeviceType(const rapidjson::Value& _value, InputDeviceType& _output);

	// キーボードの入力を列挙体に変換
	std::optional<KeyConstants> ResolveKeyboardPath(std::string_view _path);
	// マウスの入力を列挙体に変換
	MouseConstants ResolveMousePath(std::string_view _path);
	// コントローラの入力を列挙体に変換
	GamePadConstants ResolveGamePadPath(std::string_view _path);
	// パーツの役割を列挙体に変換
	InputCompositePartRole ResolveCompositePartRole(std::string_view _name);
	// デバイスを列挙体に変換
	InputDeviceType ResolveInputDeviceType(std::string_view _devicePath);

	// パスから、デバイスの種類と入力の名前を持ってくる
	bool EvaluatePath(const std::string_view& _path, std::string_view& _deviceNameOutPut, std::string_view& _controlNameOutPut);
}
