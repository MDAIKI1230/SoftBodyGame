#include <unordered_map>

#include "InputAssetDataLoadHelper.h"

namespace
{
	// キー対応表
	const std::unordered_map<std::string_view, KeyConstants> KEY_TABLE
	{
		{ "backspace", KeyConstants::BACK },
		{ "tab", KeyConstants::TAB },
		{ "enter", KeyConstants::RETURN },

		{ "leftShift", KeyConstants::LSHIFT },
		{ "rightShift", KeyConstants::RSHIFT },
		{ "leftCtrl", KeyConstants::LCONTROL },
		{ "rightCtrl", KeyConstants::RCONTROL },
		{ "escape", KeyConstants::ESCAPE },
		{ "space", KeyConstants::SPACE },
		{ "pageUp", KeyConstants::PGUP },
		{ "pageDown", KeyConstants::PGDN },
		{ "end", KeyConstants::END },
		{ "home", KeyConstants::HOME },
		{ "leftArrow", KeyConstants::LEFT },
		{ "upArrow", KeyConstants::UP },
		{ "rightArrow", KeyConstants::RIGHT },
		{ "downArrow", KeyConstants::DOWN },
		{ "insert", KeyConstants::INSERT },
		{ "delete", KeyConstants::DELETE_KEY },

		{ "minus", KeyConstants::MINUS },
		{ "yen", KeyConstants::YEN },
		{ "previousTrack", KeyConstants::PREVTRACK },
		{ "period", KeyConstants::PERIOD },
		{ "slash", KeyConstants::SLASH },
		{ "leftAlt", KeyConstants::LALT },
		{ "rightAlt", KeyConstants::RALT },
		{ "semicolon", KeyConstants::SEMICOLON },
		{ "colon", KeyConstants::COLON },
		{ "leftBracket", KeyConstants::LBRACKET },
		{ "rightBracket", KeyConstants::RBRACKET },
		{ "at", KeyConstants::AT },
		{ "backslash", KeyConstants::BACKSLASH },
		{ "comma", KeyConstants::COMMA },
		{ "capsLock", KeyConstants::CAPSLOCK },
		{ "pause", KeyConstants::PAUSE },

		{ "numpad0", KeyConstants::NUMPAD0 },
		{ "numpad1", KeyConstants::NUMPAD1 },
		{ "numpad2", KeyConstants::NUMPAD2 },
		{ "numpad3", KeyConstants::NUMPAD3 },
		{ "numpad4", KeyConstants::NUMPAD4 },
		{ "numpad5", KeyConstants::NUMPAD5 },
		{ "numpad6", KeyConstants::NUMPAD6 },
		{ "numpad7", KeyConstants::NUMPAD7 },
		{ "numpad8", KeyConstants::NUMPAD8 },
		{ "numpad9", KeyConstants::NUMPAD9 },

		{ "numpadMultiply", KeyConstants::MULTIPLY },
		{ "numpadAdd", KeyConstants::ADD },
		{ "numpadSubtract", KeyConstants::SUBTRACT },
		{ "numpadDecimal", KeyConstants::DECIMAL },
		{ "numpadDivide", KeyConstants::DIVIDE },
		{ "numpadEnter", KeyConstants::NUMPADENTER },

		{ "f1", KeyConstants::F1 },
		{ "f2", KeyConstants::F2 },
		{ "f3", KeyConstants::F3 },
		{ "f4", KeyConstants::F4 },
		{ "f5", KeyConstants::F5 },
		{ "f6", KeyConstants::F6 },
		{ "f7", KeyConstants::F7 },
		{ "f8", KeyConstants::F8 },
		{ "f9", KeyConstants::F9 },
		{ "f10", KeyConstants::F10 },
		{ "f11", KeyConstants::F11 },
		{ "f12", KeyConstants::F12 },

		{ "a", KeyConstants::A },
		{ "b", KeyConstants::B },
		{ "c", KeyConstants::C },
		{ "d", KeyConstants::D },
		{ "e", KeyConstants::E },
		{ "f", KeyConstants::F },
		{ "g", KeyConstants::G },
		{ "h", KeyConstants::H },
		{ "i", KeyConstants::I },
		{ "j", KeyConstants::J },
		{ "k", KeyConstants::K },
		{ "l", KeyConstants::L },
		{ "m", KeyConstants::M },
		{ "n", KeyConstants::N },
		{ "o", KeyConstants::O },
		{ "p", KeyConstants::P },
		{ "q", KeyConstants::Q },
		{ "r", KeyConstants::R },
		{ "s", KeyConstants::S },
		{ "t", KeyConstants::T },
		{ "u", KeyConstants::U },
		{ "v", KeyConstants::V },
		{ "w", KeyConstants::W },
		{ "x", KeyConstants::X },
		{ "y", KeyConstants::Y },
		{ "z", KeyConstants::Z },

		{ "0", KeyConstants::NUM0 },
		{ "1", KeyConstants::NUM1 },
		{ "2", KeyConstants::NUM2 },
		{ "3", KeyConstants::NUM3 },
		{ "4", KeyConstants::NUM4 },
		{ "5", KeyConstants::NUM5 },
		{ "6", KeyConstants::NUM6 },
		{ "7", KeyConstants::NUM7 },
		{ "8", KeyConstants::NUM8 },
		{ "9", KeyConstants::NUM9 }
	};
	// マウス対応表
	const std::unordered_map< std::string_view, MouseConstants> MOUSE_TABLE
	{
		// 基本ボタン
		{ "leftButton",   MouseConstants{ MouseButton::LEFT } },
		{ "rightButton",  MouseConstants{ MouseButton::RIGHT } },
		{ "middleButton", MouseConstants{ MouseButton::MIDDLE } },

		// 追加ボタン
		{ "button4",      MouseConstants{ MouseButton::BUTTON_4 } },
		{ "button5",      MouseConstants{ MouseButton::BUTTON_5 } },
		{ "button6",      MouseConstants{ MouseButton::BUTTON_6 } },
		{ "button7",      MouseConstants{ MouseButton::BUTTON_7 } },
		{ "button8",      MouseConstants{ MouseButton::BUTTON_8 } },

		// 1D Axis
		{ "scroll/x",     MouseConstants{ MouseAxis1D::SCROLL_X } },
		{ "scroll/y",     MouseConstants{ MouseAxis1D::SCROLL_Y } },

		// 2D Axis
		{ "position",     MouseConstants{ MouseAxis2D::POSITION } },
		{ "delta",        MouseConstants{ MouseAxis2D::DELTA } },
		{ "scroll",       MouseConstants{ MouseAxis2D::SCROLL } },
	};
	// ゲームパッド対応表
	const std::unordered_map<std::string_view, GamePadConstants> GAMEPAD_TABLE
	{
		// フェイスボタン
			{ "buttonSouth",     GamePadConstants{ GamePadButton::BUTTON_SOUTH } },
			{ "buttonEast",      GamePadConstants{ GamePadButton::BUTTON_EAST } },
			{ "buttonWest",      GamePadConstants{ GamePadButton::BUTTON_WEST } },
			{ "buttonNorth",     GamePadConstants{ GamePadButton::BUTTON_NORTH } },

			// ショルダーボタン
			{ "leftShoulder",    GamePadConstants{ GamePadButton::LEFT_SHOULDER } },
			{ "rightShoulder",   GamePadConstants{ GamePadButton::RIGHT_SHOULDER } },

			// スティック押し込み
			{ "leftStickPress",  GamePadConstants{ GamePadButton::LEFT_STICK_PRESS } },
			{ "rightStickPress", GamePadConstants{ GamePadButton::RIGHT_STICK_PRESS } },

			// 方向パッド
			{ "dpad/up",         GamePadConstants{ GamePadButton::DPAD_UP } },
			{ "dpad/down",       GamePadConstants{ GamePadButton::DPAD_DOWN } },
			{ "dpad/left",       GamePadConstants{ GamePadButton::DPAD_LEFT } },
			{ "dpad/right",      GamePadConstants{ GamePadButton::DPAD_RIGHT } },

			// システムボタン
			{ "select",          GamePadConstants{ GamePadButton::SELECT } },
			{ "start",           GamePadConstants{ GamePadButton::START } },
			{ "guide",           GamePadConstants{ GamePadButton::GUIDE } },

			// 1D Axis
			{ "leftTrigger",     GamePadConstants{ GamePadAxis1D::LEFT_TRIGGER } },
			{ "rightTrigger",    GamePadConstants{ GamePadAxis1D::RIGHT_TRIGGER } },

			// 2D Axis
			{ "leftStick",       GamePadConstants{ GamePadAxis2D::LEFT_STICK } },
			{ "rightStick",      GamePadConstants{ GamePadAxis2D::RIGHT_STICK } },
	};
	// パーツの役割対応表
	const std::unordered_map<std::string_view, InputCompositePartRole> COMPOSITE_PART_ROLE_TABLE
	{
		{ "Up",    InputCompositePartRole::UP },
		{ "Down",  InputCompositePartRole::DOWN },
		{ "Left",  InputCompositePartRole::LEFT },
		{ "Right", InputCompositePartRole::RIGHT }
	};
	// デバイス対応表
	const std::unordered_map<std::string_view, InputDeviceType> INPUT_DEVICE_TYPE_TABLE
	{
		{ "<Keyboard>", InputDeviceType::KEYBOARD },
		{ "<Mouse>",    InputDeviceType::MOUSE },
		{ "<GamePad>",  InputDeviceType::GAMEPAD }
	};
}

// 入力デバイスの種類取得関数
bool InputAssetDataLoadHelper::GetControlSchemes(const rapidjson::Value& _value, std::vector<InputControlSchemeData>& _output)
{
	if (!_value.HasMember("ControlSchemes") || !_value["ControlSchemes"].IsArray())
	{
		return false;
	}

	std::vector<InputControlSchemeData> loaded;

	for (auto& controlScheme : _value["ControlSchemes"].GetArray())
	{
		if (!controlScheme.IsObject())
		{
			return false;
		}

		InputControlSchemeData data;

		// ID
		if (!controlScheme.HasMember("ID") || !controlScheme["ID"].IsUint64())
		{
			return false;
		}

		data.id = InputDataID{ controlScheme["ID"].GetUint64() };

		// 名前
		if (!controlScheme.HasMember("Name") || !controlScheme["Name"].IsString())
		{
			return false;
		}

		data.name = controlScheme["Name"].GetString();

		// デバイスの入力の種類
		if (!controlScheme.HasMember("Devices") || !controlScheme["Devices"].IsArray())
		{
			return false;
		}

		for (auto& device : controlScheme["Devices"].GetArray())
		{
			if (!device.IsString())
			{
				return false;
			}

			InputDeviceType deviceType;

			if (!GetInputDeviceType(device, deviceType))
			{
				return false;
			}

			data.device.push_back(deviceType);
		}

		loaded.push_back(data);
	}

	_output = loaded;

	return true;
}

// 入力アクションマップ情報取得関数
bool InputAssetDataLoadHelper::GetActionMaps(const rapidjson::Value& _value, std::vector<InputActionMapData>& _output)
{
	if (!_value.HasMember("ActionMaps") || !_value["ActionMaps"].IsArray())
	{
		return false;
	}

	std::vector<InputActionMapData> loaded;

	for (auto& actionMap : _value["ActionMaps"].GetArray())
	{
		if (!actionMap.IsObject())
		{
			return false;
		}

		InputActionMapData data;

		// ID
		if (!actionMap.HasMember("ID") || !actionMap["ID"].IsUint64())
		{
			return false;
		}

		data.id = InputDataID{ actionMap["ID"].GetUint64() };

		// 名前
		if (!actionMap.HasMember("Name") || !actionMap["Name"].IsString())
		{
			return false;
		}

		data.name = actionMap["Name"].GetString();

		// 入力アクション
		if (!GetInputActions(actionMap, data.actions))
		{
			return false;
		}

		loaded.push_back(data);
	}

	_output = loaded;

	return true;
}
// 入力アクション情報取得関数
bool InputAssetDataLoadHelper::GetInputActions(const rapidjson::Value& _value, std::vector<InputActionData>& _output)
{
	if (!_value.HasMember("Actions") || !_value["Actions"].IsArray())
	{
		return false;
	}

	std::vector<InputActionData> loaded;

	for (auto& action : _value["Actions"].GetArray())
	{
		if (!action.IsObject())
		{
			return false;
		}

		InputActionData data;

		// ID
		if (!action.HasMember("ID") || !action["ID"].IsUint64())
		{
			return false;
		}

		data.id = InputDataID{ action["ID"].GetUint64() };

		// 名前
		if (!action.HasMember("Name") || !action["Name"].IsString())
		{
			return false;
		}
		
		data.name = action["Name"].GetString();

		// 入力の扱い方
		if (!GetInputActionType(action, data.type))
		{
			return false;
		}

		// 最終的な入力値の設定
		if (!GetInputValueType(action, data.valueType))
		{
			return false;
		}


		// 入力バインド
		if (!GetInteraction(action, data.interaction))
		{
			return false;
		}

		// 入力バインド
		if (!GetInputBinding(action, data.bindings))
		{
			return false;
		}

		loaded.push_back(data);
	}

	_output = loaded;

	return true;
}

// 発動タイミング情報取得関数
bool InputAssetDataLoadHelper::GetInteraction(const rapidjson::Value& _value, InputInteractionData& _output)
{
	InputInteractionData loaded;

	// Interaction省略時はPressとして扱う
	if (!_value.HasMember("Interaction"))
	{
		_output = loaded;
		return true;
	}

	if (!_value["Interaction"].IsObject())
	{
		return false;
	}

	const rapidjson::Value& interaction{ _value["Interaction"] };

	// 発動タイミング種類
	if (!GetInputInteractionType(interaction, loaded.type))
	{
		return false;
	}

	switch (loaded.type)
	{
	case InputInteractionType::PRESS:
		_output = loaded;

		return true;
		break;
	case InputInteractionType::HOLD:
	case InputInteractionType::TAP:
		if (!interaction.HasMember("Duration") || !interaction["Duration"].IsNumber())
		{
			return false;
		}

		loaded.duration = interaction["Duration"].GetFloat();

		if (loaded.duration <= 0.0f)
		{
			return false;
		}

		_output = loaded;

		return true;
		break;
	default:
		return false;
		break;
	}
}

// 入力バインド情報取得関数
bool InputAssetDataLoadHelper::GetInputBinding(const rapidjson::Value& _value, std::vector<InputBindingData>& _output)
{
	if (!_value.HasMember("Bindings") || !_value["Bindings"].IsArray())
	{
		return false;
	}

	std::vector<InputBindingData> loaded;

	for (auto& binding : _value["Bindings"].GetArray())
	{
		if (!binding.IsObject())
		{
			return false;
		}

		InputBindingData data;

		// ID
		if (!binding.HasMember("ID") || !binding["ID"].IsUint64())
		{
			return false;
		}

		data.id = InputDataID{ binding["ID"].GetUint64() };

		// 名前
		if (!binding.HasMember("Name") || !binding["Name"].IsString())
		{
			return false;
		}

		data.name = binding["Name"].GetString();

		// 入力デバイスID
		if (!binding.HasMember("SchemeIDs") || !binding["SchemeIDs"].IsArray())
		{
			return false;
		}

		for (auto& schemeId : binding["SchemeIDs"].GetArray())
		{
			if (!schemeId.IsUint64())
			{
				return false;
			}

			data.schemeIds.push_back(InputDataID{ schemeId.GetUint64() });
		}

		// 入力を作るためのバインド情報
		if (!GetBindingSource(binding, data.source))
		{
			return false;
		}

		// デッドゾーン
		if (binding.HasMember("DeadZone") && binding["DeadZone"].IsNumber())
		{
			data.deadZone = binding["DeadZone"].GetFloat();
			if (data.deadZone < 0.0f)
			{
				data.deadZone = 0.0f;
			}

			if (data.deadZone >= 1.0f)
			{
				data.deadZone = 0.9999f;
			}
		}

		loaded.push_back(data);
	}

	_output = loaded;

	return true;
}

// Binding固有情報取得関数
bool InputAssetDataLoadHelper::GetBindingSource(const rapidjson::Value& _value, InputBindingSourceData& _output)
{
	// Partsの数チェック
	if ((!_value.HasMember("Kind") || !_value["Kind"].IsString()))
	{
		return false;
	}

	// 単一
	std::string_view kind{ _value["Kind"].GetString(),_value["Kind"].GetStringLength() };
	if (kind == "Control")
	{
		return GetControlBinding(_value, _output);
	}

	// 複数
	if (kind == "Composite")
	{
		return GetCompositeBinding(_value, _output);
	}

	return false;
}

// Control版(単一)Binding固有情報取得関数
bool InputAssetDataLoadHelper::GetControlBinding(const rapidjson::Value& _value, InputBindingSourceData& _output)
{
	InputControlBindingData loaded;
	// 解決済みの入力Control
	if (!GetResolvedInputControl(_value, loaded.control))
	{
		return false;
	}

	_output = loaded;

	return true;
}

// Composite版(複数)Binding固有情報取得関数
bool InputAssetDataLoadHelper::GetCompositeBinding(const rapidjson::Value& _value, InputBindingSourceData& _output)
{
	InputCompositeBindingData loaded;

	if (!GetInputCompositeType(_value, loaded.type))
	{
		return false;
	}

	if (!GetBindingParts(_value, loaded.parts))
	{
		return false;
	}

	_output = loaded;

	return true;
}

// Part情報取得関数
bool InputAssetDataLoadHelper::GetBindingPart(const rapidjson::Value& _value, InputBindingPartData& _output)
{
	if (!_value.IsObject())
	{
		return false;
	}

	InputBindingPartData loaded;

	// ID
	if (!_value.HasMember("ID") || !_value["ID"].IsUint64())
	{
		return false;
	}

	loaded.id = InputDataID{ _value["ID"].GetUint64() };

	// 名前
	if (!GetInputCompositePartRole(_value, loaded.role))
	{
		return false;
	}

	// 解決済みの入力Control
	if (!GetResolvedInputControl(_value, loaded.control))
	{
		return false;
	}

	_output = loaded;

	return true;
}

// Parts情報取得関数
bool InputAssetDataLoadHelper::GetBindingParts(const rapidjson::Value& _value, std::vector<InputBindingPartData>& _output)
{
	// 入力Controlへのパス
	if (!_value.HasMember("Parts") || !_value["Parts"].IsArray())
	{
		return false;
	}

	std::vector<InputBindingPartData> loaded;

	for (auto& part : _value["Parts"].GetArray())
	{
		InputBindingPartData data;
		if (!GetBindingPart(part, data))
		{
			return false;
		}

		loaded.push_back(data);
	}

	// 一つも入っていないなら取得失敗とする
	if (loaded.size() != 0)
	{
		_output = loaded;
		return true;
	}
	else
	{
		return false;
	}
}

// 発動タイミング種類をJsonから取ってくる
bool InputAssetDataLoadHelper::GetInputInteractionType(const rapidjson::Value& _value, InputInteractionType& _output)
{
	if (!_value.HasMember("Type") || !_value["Type"].IsString())
	{
		return false;
	}

	std::string_view interactionType{ _value["Type"].GetString(),_value["Type"].GetStringLength() };

	// 押した瞬間にPerformed
	if (interactionType == "Press")
	{
		_output = InputInteractionType::PRESS;

		return true;
	}

	// 指定時間押し続けたらPerformed
	if (interactionType == "Hold")
	{		
		_output = InputInteractionType::HOLD;

		return true;
	}

	// 指定時間以内に離したらPerformed
	if (interactionType == "Tap")
	{
		_output = InputInteractionType::TAP;

		return true;
	}

	return false;
}

// InputActionTypeをJsonから取ってくる
bool InputAssetDataLoadHelper::GetInputActionType(const rapidjson::Value& _value, InputActionType& _output)
{
	// チェック
	if (!_value.HasMember("Type") || !_value["Type"].IsString())
	{
		return false;
	}

	std::string_view type{ _value["Type"].GetString(),_value["Type"].GetStringLength() };

	if (type == "Button")
	{
		_output = InputActionType::BUTTON;
		return true;
	}

	if (type == "Value")
	{
		_output = InputActionType::VALUE;
		return true;
	}

	if (type == "PassThrough")
	{
		_output = InputActionType::PASS_THROUGH;
		return true;
	}

	return false;
}

// InputValueTypeをJsonから取ってくる
bool InputAssetDataLoadHelper::GetInputValueType(const rapidjson::Value& _value, InputValueType& _output)
{
	// チェック
	if (!_value.HasMember("ValueType") || !_value["ValueType"].IsString())
	{
		return false;
	}

	std::string_view valueType{ _value["ValueType"].GetString(),_value["ValueType"].GetStringLength() };

	if (valueType == "Bool")
	{
		_output = InputValueType::BOOL;
		return true;
	}

	if (valueType == "Float")
	{
		_output = InputValueType::FLOAT;
		return true;
	}

	if (valueType == "Vector2")
	{
		_output = InputValueType::VECTOR2;
		return true;
	}

	return false;
}

// InputCompositeTypeをJsonから取ってくる
bool InputAssetDataLoadHelper::GetInputCompositeType(const rapidjson::Value& _value, InputCompositeType& _output)
{
	// チェック
	if (!_value.HasMember("CompositeType") || !_value["CompositeType"].IsString())
	{
		return false;
	}

	std::string_view compositeType{ _value["CompositeType"].GetString(),_value["CompositeType"].GetStringLength() };

	if (compositeType == "Axis2d")
	{
		_output = InputCompositeType::AXIS2D;
		return true;
	}

	return false;
}

// ResolvedInputControlをJsonから取ってくる
bool InputAssetDataLoadHelper::GetResolvedInputControl(const rapidjson::Value& _value, ResolvedInputControl& _output)
{
	// チェック
	if (!_value.HasMember("Path") || !_value["Path"].IsString())
	{
		return false;
	}

	// デバイスの種類
	std::string_view deviceName;
	// 入力の名前
	std::string_view controlName;

	if (!EvaluatePath(_value["Path"].GetString(), deviceName, controlName))
	{
		return false;
	}

	if (deviceName == "Keyboard")
	{
		if (controlName == "anyKey")
		{
			_output = AnyButtonControl{ InputDeviceType::KEYBOARD };

			return true;
		}

		std::optional<KeyConstants> key{ ResolveKeyboardPath(controlName) };

		if (key.has_value())
		{
			_output = key.value();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (deviceName == "Mouse")
	{
		if (controlName == "anyButton")
		{
			_output = AnyButtonControl{ InputDeviceType::MOUSE };

			return true;
		}

		MouseConstants control{ ResolveMousePath(controlName) };

		if (std::holds_alternative<std::monostate>(control))
		{
			return false;
		}

		_output = control;

		return true;
	}
	else if (deviceName == "GamePad")
	{
		if (controlName == "anyButton")
		{
			_output = AnyButtonControl{ InputDeviceType::GAMEPAD };

			return true;
		}

		GamePadConstants control{ ResolveGamePadPath(controlName) };

		if (std::holds_alternative<std::monostate>(control))
		{
			return false;
		}

		_output = control;

		return true;
	}
	else
	{
		_output = std::monostate{};
		return false;
	}
}

// パーツの役割をJsonから取ってくる
bool InputAssetDataLoadHelper::GetInputCompositePartRole(const rapidjson::Value& _value, InputCompositePartRole& _output)
{
	// チェック
	if (!_value.HasMember("Role") || !_value["Role"].IsString())
	{
		return false;
	}

	InputCompositePartRole loaded{ ResolveCompositePartRole(_value["Role"].GetString()) };

	if (loaded == InputCompositePartRole::INVALID)
	{
		return false;
	}

	_output = loaded;

	return true;
}

// デバイス種類をJsonから取ってくる
bool InputAssetDataLoadHelper::GetInputDeviceType(const rapidjson::Value& _value, InputDeviceType& _output)
{
	if (!_value.IsString())
	{
		return false;
	}

	std::string_view devicePath{ _value.GetString(),_value.GetStringLength() };

	InputDeviceType loaded{ ResolveInputDeviceType(devicePath) };

	if (loaded == InputDeviceType::INVALID)
	{
		return false;
	}

	_output = loaded;

	return true;
}

// パスから、デバイスの種類と入力の名前を持ってくる
bool InputAssetDataLoadHelper::EvaluatePath(const std::string_view& _path, std::string_view& _deviceNameOutPut, std::string_view& _controlNameOutPut)
{
	// 最初は必ず <
	if (_path.empty() || _path.front() != '<')
	{
		return false;
	}

	// 閉じる > を探す
	std::size_t closePos{ _path.find('>') };

	if (closePos == std::string_view::npos ||
		closePos == 1)
	{
		return false;
	}

	// > の直後が / か確認
	if (closePos + 1 >= _path.size() || _path[closePos + 1] != '/')
	{
		return false;
	}
	// デバイスの種類
	_deviceNameOutPut = _path.substr(1, closePos - 1);
	// 入力の名前
	_controlNameOutPut = _path.substr(closePos + 2);

	return true;
}

// キーボードの入力を列挙体に変換
std::optional<KeyConstants> InputAssetDataLoadHelper::ResolveKeyboardPath(std::string_view _path)
{
	const auto iterator = KEY_TABLE.find(_path);

	if (iterator == KEY_TABLE.end())
	{
		return std::nullopt;
	}

	return iterator->second;
}
// マウスの入力を列挙体に変換
MouseConstants InputAssetDataLoadHelper::ResolveMousePath(std::string_view _path)
{
	const auto iterator = MOUSE_TABLE.find(_path);

	if (iterator == MOUSE_TABLE.end())
	{
		return MouseConstants{ std::monostate{} };
	}

	return iterator->second;
}
// コントローラの入力を列挙体に変換
GamePadConstants InputAssetDataLoadHelper::ResolveGamePadPath(std::string_view _path)
{
	const auto iterator = GAMEPAD_TABLE.find(_path);

	if (iterator == GAMEPAD_TABLE.end())
	{
		return GamePadConstants{ std::monostate{} };
	}

	return iterator->second;
}
// パーツの役割を列挙体に変換
InputCompositePartRole InputAssetDataLoadHelper::ResolveCompositePartRole(std::string_view _name)
{
	const auto iterator =
		COMPOSITE_PART_ROLE_TABLE.find(_name);

	if (iterator == COMPOSITE_PART_ROLE_TABLE.end())
	{
		return InputCompositePartRole::INVALID;
	}

	return iterator->second;
}

InputDeviceType InputAssetDataLoadHelper::ResolveInputDeviceType(std::string_view _devicePath)
{
	const auto iterator{ INPUT_DEVICE_TYPE_TABLE.find(_devicePath) };

	if (iterator == INPUT_DEVICE_TYPE_TABLE.end())
	{
		return InputDeviceType::INVALID;
	}

	return iterator->second;
}
