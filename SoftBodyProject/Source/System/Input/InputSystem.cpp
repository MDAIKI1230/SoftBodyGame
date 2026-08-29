#include "ServiceLocator.h"

#include "InputAssetDataLoader.h"

#include "InputSystem.h"

bool InputSystem::LoadAsset(std::string _path)
{
	if (InputAssetDataLoader::LoadAsset(_path, data))
	{
		return BuildRuntimeState();
	}
	else
	{
		return false;
	}
}

void InputSystem::Update()
{
	ApplyInputAction();
	nativeInput->Update();
	EvaluateActions();
	UpdateInteractions();
}

// InputAction取得関数
InputAction InputSystem::GetInputAction(std::string_view _actionMapName, std::string_view _inputActionName)
{
	for (const auto& actionMap : data.actionMaps)
	{
		if (actionMap.name == _actionMapName)
		{
			for (const auto& action : actionMap.actions)
			{
				if (action.name == _inputActionName)
				{
					if (action.id.IsValid())
					{

						return { action.id };
					}
				}
			}
		}
	}

	return InputAction{};
}

// InputAction達のApplyを呼ぶ
void InputSystem::ApplyInputAction()
{
	// 死んだ奴らを弔う
	for (auto& [id, runtimeData] : runtimeDatas)
	{
		ApplyRemove(runtimeData.callbacks.started);
		ApplyRemove(runtimeData.callbacks.performed);
		ApplyRemove(runtimeData.callbacks.canceled);
	}
	for (auto& [id, pendingAddList] : pendingAddLists)
	{
		ApplyRemove(pendingAddList.started);
		ApplyRemove(pendingAddList.performed);
		ApplyRemove(pendingAddList.canceled);
	}

	// 追加
	for (auto& [id, pendingAddList] : pendingAddLists)
	{
		InputActionCallbacks& callbacks{ runtimeDatas[id].callbacks };

		ApplyAdd(callbacks.started, pendingAddList.started);
		ApplyAdd(callbacks.performed, pendingAddList.performed);
		ApplyAdd(callbacks.canceled, pendingAddList.canceled);
	}

	pendingAddLists.clear();
}

// ロードしたデータから状態管理用構造体を作る関数
bool InputSystem::BuildRuntimeState()
{
	std::unordered_map<std::uint64_t, InputActionRuntimeData> loadedStates;

	for (const auto& actionMap : data.actionMaps)
	{
		for (const auto& action : actionMap.actions)
		{
			if (!action.id.IsValid())
			{
				return false;
			}

			InputValue defaultValue = MakeDefaultValue(action.valueType);

			// 値を保持しているか確認
			if (std::holds_alternative<std::monostate>(defaultValue))
			{
				return false;
			}

			// Stateを作る
			InputActionRuntimeState state;
			state.previous = defaultValue;
			state.current = defaultValue;
			state.phase = InputActionPhase::WAITING;

			// 値の重複チェック
			const auto [iterator, inserted] = loadedStates.emplace(action.id.value, std::move(state));

			if (!inserted)
			{
				return false;
			}
		}
	}

	runtimeDatas = std::move(loadedStates);
	return true;
}

// InputValueを入力を評価して作る関数
void InputSystem::EvaluateActions()
{
	for (const auto& actionMap : data.actionMaps)
	{
		for (const auto& action : actionMap.actions)
		{
			auto stateIt = runtimeDatas.find(action.id.value);

			// Build済みなら本来起こらない
			if (stateIt == runtimeDatas.end())
			{
				continue;
			}

			InputActionRuntimeState& state{ stateIt->second.state };

			state.previous = state.current;
			state.current = MakeDefaultValue(action.valueType);

			for (const auto& binding : action.bindings)
			{
				// 新たな値を評価
				InputValue bindingValue = EvaluateBinding(binding);

				// 空チェック
				if (std::holds_alternative<std::monostate>(bindingValue))
				{
					continue;
				}

				// 値を新しくする
				MergeInputValue(action.valueType, bindingValue, state.current, binding.deadZone);
			}
		}
	}
}

// 作られたInputValueから、ActionのPhaseや時間の更新をする関数
void InputSystem::UpdateInteractions()
{
	for (const auto& actionMap : data.actionMaps)
	{
		for (const auto& action : actionMap.actions)
		{
			auto stateIt = runtimeDatas.find(action.id.value);

			// Build済みなら本来起こらない
			if (stateIt == runtimeDatas.end())
			{
				continue;
			}

			InputActionRuntimeState& state{ stateIt->second.state };

			InputInteractionEvents events;

			switch (action.interaction.type)
			{
			case InputInteractionType::PRESS:
				events = UpdateInteractionPress(state);
				break;
			case InputInteractionType::HOLD:
				events = UpdateInteractionHold(state, action.interaction.duration);
				break;
			case InputInteractionType::TAP:
				events = UpdateInteractionTap(state, action.interaction.duration);
				break;
			default:
				break;
			}

			// イベントフラグが立ってる場合のみ発動
			if (events.started || events.performed || events.canceled)
			{
				InvokePhaseCallbacks(stateIt->second, events);
			}
		}
	}
}

// 追加
void InputSystem::ApplyAdd(std::vector<InputCallbackEntry>& _callbacks, std::vector<InputCallbackEntry>& _pendingAdds)
{
	for (auto& add : _pendingAdds)
	{
		_callbacks.push_back(add);
	}
}
// 削除
void InputSystem::ApplyRemove(std::vector<InputCallbackEntry>& _callbacks)
{
	std::erase_if(
		_callbacks,
		[](const InputCallbackEntry& _callback)
		{
			return !_callback.active;
		});
}

// InputValueTypeから初期値の対応したInputValueを作る関数
InputValue InputSystem::MakeDefaultValue(InputValueType _type)
{
	switch (_type)
	{
	case InputValueType::BOOL:
		return false;
	case InputValueType::FLOAT:
		return 0.0f;
	case InputValueType::VECTOR2:
		return Vector2{};
	default:
		return std::monostate{};
	}
}

// バインドからInputValue作成関数
InputValue InputSystem::EvaluateBinding(const InputBindingData& _binding)
{
	if (const auto* control = std::get_if<InputControlBindingData>(&_binding.source))
	{
		return EvaluateControl(*control);
	}

	if (const auto* composite = std::get_if<InputCompositeBindingData>(&_binding.source))
	{
		return EvaluateComposite(*composite);
	}

	return std::monostate{};
}

// 入力が単一の場合のバインド作成関数
InputValue InputSystem::EvaluateControl(const InputControlBindingData& _binding)
{
	if (const auto* control = std::get_if<KeyConstants>(&_binding.control))
	{
		return EvaluateKeyboardInput(*control);
	}

	if (const auto* composite = std::get_if<MouseConstants>(&_binding.control))
	{
		return EvaluateMouseInput(*composite);
	}

	if (const auto* composite = std::get_if<GamePadConstants>(&_binding.control))
	{
		return EvaluateGamePadInput(*composite);
	}

	return std::monostate{};
}
// 入力が複数の場合のバインド作成関数
InputValue InputSystem::EvaluateComposite(const InputCompositeBindingData& _binding)
{
	switch (_binding.type)
	{
	case InputCompositeType::AXIS2D:
		return EvaluateAxis2d(_binding);
	case InputCompositeType::INVALID:
	default:
		return std::monostate{};
		break;
	}
	
}

// キーボード列挙体を評価
InputValue InputSystem::EvaluateKeyboardInput(KeyConstants _input)
{
	// 値を取得
	return InputValue{ nativeInput->GetCurrentValue(_input) };
}
// マウス列挙体を評価
InputValue InputSystem::EvaluateMouseInput(MouseConstants _input)
{
	if (auto button = std::get_if<MouseButton>(&_input))
	{
		return InputValue{ nativeInput->GetCurrentValue(*button) };
	}

	if (auto axis = std::get_if<MouseAxis1D>(&_input))
	{
		return InputValue{ nativeInput->GetCurrentValue(*axis) };
	}

	if (auto axis = std::get_if<MouseAxis2D>(&_input))
	{
		return InputValue{ nativeInput->GetCurrentValue(*axis) };
	}

	return std::monostate{};
}
// コントローラ列挙体を評価
InputValue InputSystem::EvaluateGamePadInput(GamePadConstants _input)
{
	if (auto button = std::get_if<GamePadButton>(&_input))
	{
		return InputValue{ nativeInput->GetCurrentValue(*button,activeGamePadIndex) };
	}

	if (auto axis = std::get_if<GamePadAxis1D>(&_input))
	{
		return InputValue{ nativeInput->GetCurrentValue(*axis,activeGamePadIndex) };
	}

	if (auto axis = std::get_if<GamePadAxis2D>(&_input))
	{
		return InputValue{ nativeInput->GetCurrentValue(*axis,activeGamePadIndex) };
	}

	return std::monostate{};
}

// InputCompositeTypeがAXIS2Dだった時の関数
InputValue InputSystem::EvaluateAxis2d(const InputCompositeBindingData& _binding)
{
	Vector2 result{};

	for (const auto& part : _binding.parts)
	{
		ResolvedInputControl controll{ part.control };

		InputValue inputValue;

		if (const auto* control = std::get_if<KeyConstants>(&controll))
		{
			inputValue = EvaluateKeyboardInput(*control);
		}
		else if (const auto* composite = std::get_if<MouseConstants>(&controll))
		{
			inputValue = EvaluateMouseInput(*composite);
		}
		else if (const auto* composite = std::get_if<GamePadConstants>(&controll))
		{
			inputValue = EvaluateGamePadInput(*composite);
		}
		else
		{
			inputValue = std::monostate{};
		}

		// 対応する値を追加
		if (auto input = std::get_if<bool>(&inputValue))
		{
			if (*input == true)
			{
				switch (part.role)
				{
				case InputCompositePartRole::UP:
					result.y += 1.0f;
					break;
				case InputCompositePartRole::DOWN:
					result.y += -1.0f;
					break;
				case InputCompositePartRole::RIGHT:
					result.x += 1.0f;
					break;
				case InputCompositePartRole::LEFT:
					result.x += -1.0f;
					break;
				case InputCompositePartRole::INVALID:
				default:
					break;
				}
			}
		}
	}

	return result;
}

// 現在の値を新しいものにする
void InputSystem::MergeInputValue(InputValueType _valueType, const InputValue& _newValue, InputValue& _current, const float _threshold = 0.0f)
{
	// 値の種類によっての操作
	switch (_valueType)
	{
	// どちらか真なら真
	case InputValueType::BOOL:
	{
		const auto* newValue{ std::get_if<bool>(&_newValue) };

		auto* current{ std::get_if<bool>(&_current) };

		if (newValue && current)
		{
			*current = *current || *newValue;
		}

		break;
	}
	// 絶対値の大きいほうを正しい値とする
	case InputValueType::FLOAT:
	{
		const auto* newValue{ std::get_if<float>(&_newValue) };
		auto* current{ std::get_if<float>(&_current) };

		// 値がちゃんと正しい型かチェック
		if (!newValue || !current)
		{
			break;
		}

		// デッドゾーン判定
		float length{ std::abs(*newValue) };

		if (length <= _threshold)
		{
			break;
		}

		float processedLength{ (length - _threshold) / (1.0f - _threshold) };

		processedLength = std::copysign(processedLength, *newValue);

		if (std::abs(processedLength) > std::abs(*current))
		{
			*current = processedLength;
		}

		break;
	}
	// ノルムが大きほうを正しい値とする
	case InputValueType::VECTOR2:
	{
		const auto* newValue{ std::get_if<Vector2>(&_newValue) };
		auto* current{ std::get_if<Vector2>(&_current) };
		// 値がちゃんと正しい型かチェック
		if (!newValue || !current)
		{
			break;
		}

		// デッドゾーン判定
		float length{ newValue->Length() };

		if (length <= _threshold)
		{
			break;
		}

		float processedLength{ (length - _threshold) / (1.0f - _threshold) };

		Vector2 candidate{ (*newValue / length) * processedLength };

		if (candidate.LengthSqr() > current->LengthSqr())
		{
			*current = candidate;
		}

		break;
	}

	default:
		break;
	}
}

// PRESSの評価関数
InputInteractionEvents InputSystem::UpdateInteractionPress(InputActionRuntimeState& _state)
{
	InputInteractionEvents result;

	if (_state.phase == InputActionPhase::CANCELED)
	{
		_state.phase = InputActionPhase::WAITING;
		_state.interactionTime = 0.0f;
	}

	bool currentActuated{ IsActuated(_state.current) };
	bool previousActuated{ IsActuated(_state.previous) };

	switch (_state.phase)
	{
	case InputActionPhase::DISABLED:
		return result;

	case InputActionPhase::WAITING:
		if (currentActuated && !previousActuated)
		{
			_state.phase = InputActionPhase::PERFORMED;
			result.started = true;
			result.performed = true;
		}
		break;

	case InputActionPhase::PERFORMED:
		if (currentActuated)
		{
			_state.interactionTime += ServiceLocator::GetTimeManager()->GetDeltaTime();
		}

		if (!currentActuated && previousActuated)
		{
			_state.phase = InputActionPhase::CANCELED;
			result.canceled = true;
		}
		break;

	case InputActionPhase::STARTED:
	case InputActionPhase::CANCELED:
	default:
		break;
	}

	return result;
}
// HOLDの評価関数
InputInteractionEvents InputSystem::UpdateInteractionHold(InputActionRuntimeState& _state, float _duration)
{
	InputInteractionEvents result;

	if (_state.phase == InputActionPhase::CANCELED)
	{
		_state.phase = InputActionPhase::WAITING;
		_state.interactionTime = 0.0f;
	}

	bool currentActuated{ IsActuated(_state.current) };
	bool previousActuated{ IsActuated(_state.previous) };

	switch (_state.phase)
	{
	case InputActionPhase::DISABLED:
		return result;
		break;
	case InputActionPhase::WAITING:
		// 現在の入力がないなら値をリセット
		if (!currentActuated)
		{
			_state.interactionTime = 0.0f;
		}
		// 押し始め判定
		if (currentActuated && !previousActuated)
		{
			_state.phase = InputActionPhase::STARTED;
			result.started = true;
		}
		break;
	case InputActionPhase::STARTED:
		if (currentActuated && previousActuated)
		{
			_state.interactionTime += ServiceLocator::GetTimeManager()->GetDeltaTime();
		}
		// 秒数判定
		if (_state.interactionTime >= _duration)
		{
			_state.phase = InputActionPhase::PERFORMED;
			result.performed = true;
		}

		// 離された判定
		if (!currentActuated && previousActuated)
		{
			_state.phase = InputActionPhase::CANCELED;
			result.canceled = true;
		}
		break;
	case InputActionPhase::PERFORMED:
		if (currentActuated)
		{
			_state.interactionTime += ServiceLocator::GetTimeManager()->GetDeltaTime();
		}

		if (!currentActuated && previousActuated)
		{
			// interactionTimeは残しておく
			_state.phase = InputActionPhase::CANCELED;
			result.canceled = true;
		}
		break;
	case InputActionPhase::CANCELED:
	default:
		break;
	}

	return result;
}
// TAPの評価関数
InputInteractionEvents InputSystem::UpdateInteractionTap(InputActionRuntimeState& _state, float _duration)
{
	InputInteractionEvents result;

	if (_state.phase == InputActionPhase::PERFORMED ||
		_state.phase == InputActionPhase::CANCELED)
	{
		_state.phase = InputActionPhase::WAITING;
		_state.interactionTime = 0.0f;
	}

	bool currentActuated{ IsActuated(_state.current) };
	bool previousActuated{ IsActuated(_state.previous) };

	switch (_state.phase)
	{
	case InputActionPhase::DISABLED:
		return result;
		break;
	case InputActionPhase::WAITING:
		// 押し始め判定
		if (!currentActuated)
		{
			_state.interactionTime = 0.0f;
		}
		if (currentActuated && !previousActuated)
		{
			_state.phase = InputActionPhase::STARTED;
			result.started = true;
		}
		else if (!currentActuated && previousActuated)
		{
			_state.phase = InputActionPhase::CANCELED;
			result.canceled = true;
		}
		break;
	case InputActionPhase::STARTED:
		if (currentActuated)
		{
			_state.interactionTime += ServiceLocator::GetTimeManager()->GetDeltaTime();
		}

		// 離された判定
		if (!currentActuated && previousActuated)
		{
			if (_state.interactionTime <= _duration)
			{
				_state.phase = InputActionPhase::PERFORMED;
				result.performed = true;
			}
			else
			{
				_state.phase = InputActionPhase::CANCELED;
				result.canceled = true;
			}
		}
		break;
	case InputActionPhase::PERFORMED:
	case InputActionPhase::CANCELED:
		break;
	default:
		break;
	}

	return result;
}

bool InputSystem::IsActuated(const InputValue& _value, const float _threshold) const
{
	// そのまま値を返す
	if (const auto* value = std::get_if<bool>(&_value))
	{
		return *value;
	}

	// 絶対値が閾値より大きいなら真
	if (const auto* value = std::get_if<float>(&_value))
	{
		return std::abs(*value) > _threshold;
	}

	// ノルムが閾値より大きいなら真
	if (const auto* value = std::get_if<Vector2>(&_value))
	{
		return value->LengthSqr() > _threshold * _threshold;
	}

	return false;
}

// 対応するコールバック関数をすべて呼ぶ
void InputSystem::InvokePhaseCallbacks(InputActionRuntimeData& _runtimeData, const InputInteractionEvents& _events)
{
	InputActionContext context;
	context.currentValue = _runtimeData.state.current;
	context.previousValue = _runtimeData.state.previous;
	context.duration = _runtimeData.state.interactionTime;

	if (_events.started)
	{
		InvokeCallbacks(_runtimeData.callbacks.started, context);
	}

	if (_events.performed)
	{
		InvokeCallbacks(_runtimeData.callbacks.performed, context);
	}
	if(_events.canceled)
	{
		InvokeCallbacks(_runtimeData.callbacks.canceled, context);
	}
}

void InputSystem::InvokeCallbacks(std::vector<InputCallbackEntry>& _callbacks, InputActionContext& _context)
{
	for (const auto& callback : _callbacks)
	{
		// アクティブチェック
		if (callback.active)
		{
			// 持ち主が死んでないかチェック
			if (callback.userData)
			{
				callback.function(callback.userData, _context);
			}
		}
	}
}

// Startedの追加待ちに追加
bool InputSystem::QueueStartedCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
	if (!_id.IsValid())
	{
		return false;
	}

	return AddCallback(pendingAddLists[_id.value].started, _func, _user);
}
// Performedの追加待ちに追加
bool InputSystem::QueuePerformedCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
	if (!_id.IsValid())
	{
		return false;
	}

	return AddCallback(pendingAddLists[_id.value].performed, _func, _user);
}
// Canceledの追加待ちに追加
bool InputSystem::QueueCanceledCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
{
	if (!_id.IsValid())
	{
		return false;
	}

	return AddCallback(pendingAddLists[_id.value].canceled, _func, _user);
}

bool InputSystem::AddCallback(std::vector<InputCallbackEntry>& _pendingAddList, InputCallbackEntry::InputActionCallbackFunc _function, void* _userData)
{
	if (_function == nullptr)
	{
		return false;
	}

	_pendingAddList.push_back({ _function,_userData });

	return true;
}

// Startedの解除待ちにする
bool InputSystem::StartedCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _userData)
{
	if (!_id.IsValid())
	{
		return false;
	}

	if (RemoveCallback(runtimeDatas[_id.value].callbacks.started, _func, _userData))
	{
		return true;
	}

	if (RemoveCallback(pendingAddLists[_id.value].started, _func, _userData))
	{
		return true;
	}
	return false;
}
// Performedの解除待ちにする
bool InputSystem::PerformedCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _userData)
{
	if (!_id.IsValid())
	{
		return false;
	}

	if (RemoveCallback(runtimeDatas[_id.value].callbacks.performed, _func, _userData))
	{
		return true;
	}

	if (RemoveCallback(pendingAddLists[_id.value].performed, _func, _userData))
	{
		return true;
	}
	return false;
}
// Canceledの解除待ちにする
bool InputSystem::CanceledCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _userData)
{
	if (!_id.IsValid())
	{
		return false;
	}

	if (RemoveCallback(runtimeDatas[_id.value].callbacks.canceled, _func, _userData))
	{
		return true;
	}

	if (RemoveCallback(pendingAddLists[_id.value].canceled, _func, _userData))
	{
		return true;
	}
	return false;
}

bool InputSystem::RemoveCallback(std::vector<InputCallbackEntry>& _callbacks, InputCallbackEntry::InputActionCallbackFunc _function, void* _userData)
{
	const auto iterator = std::find_if(
		_callbacks.begin(),
		_callbacks.end(),
		[_function, _userData](const InputCallbackEntry& _callback)
		{
			return _callback.active &&
				_callback.function == _function &&
				_callback.userData == _userData;
		});

	if (iterator == _callbacks.end())
	{
		return false;
	}

	iterator->active = false;
	return true;
}
