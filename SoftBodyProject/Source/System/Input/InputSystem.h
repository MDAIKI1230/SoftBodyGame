#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <optional>

#include "KeyConstants.h"
#include "MouseConstants.h"
#include "GamePadConstants.h"

#include "IInput.h"

#include "Struct/InputActionAssetData.h"
#include "Struct/InputActionRuntimeData.h"
#include "Struct/InputInteractionEvents.h"
#include "InputAction.h"
#include "InputActionContext.h"

class InputSystem
{
public:
	class CallbackAccessKey
	{
		friend class InputAction;

	private:
		CallbackAccessKey() = default;
	};
public:
	InputSystem(std::unique_ptr<IInput>&& _nativeInput) :
		nativeInput{ std::move(_nativeInput) }
	{
	}
	// アセットの読み込み
	bool LoadAsset(std::string _path);
	// 更新処理
	void Update();
	// InputAction取得関数
	InputAction GetInputAction(std::string_view _actionMapName, std::string_view _inputActionName);


	// Startedの追加待ちに追加
	bool QueueStartedCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
	// Performedの追加待ちに追加
	bool QueuePerformedCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user);
	// Canceledの追加待ちに追加
	bool QueueCanceledCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user);

	// Startedの解除待ちにする
	bool StartedCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _userData);
	// Performedの解除待ちにする
	bool PerformedCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _userData);
	// Canceledの解除待ちにする
	bool CanceledCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _userData);

private:
	// InputAction達のApplyを呼ぶ
	void ApplyInputAction();
	// ロードしたデータから状態管理用構造体を作る関数
	bool BuildRuntimeState();
	// InputValueを入力を評価して作る関数
	void EvaluateActions();
	// 作られたInputValueから、ActionのPhaseや時間の更新をする関数
	void UpdateInteractions();

	// 追加
	void ApplyAdd(std::vector<InputCallbackEntry>& _callbacks, std::vector<InputCallbackEntry>& _pendingAdds);
	// 削除
	void ApplyRemove(std::vector<InputCallbackEntry>& _callbacks);

	// InputValueTypeから初期値の対応したInputValueを作る関数
	InputValue MakeDefaultValue(InputValueType _type);

	// バインドからInputValue作成関数
	InputValue EvaluateBinding(const InputBindingData& _binding);

	// 入力が単一の場合のバインド作成関数
	InputValue EvaluateControl(const InputControlBindingData& _binding);
	// 入力が複数の場合のバインド作成関数
	InputValue EvaluateComposite(const InputCompositeBindingData& _binding);

	// キーボード列挙体を評価
	InputValue EvaluateKeyboardInput(KeyConstants _input);
	// マウス列挙体を評価
	InputValue EvaluateMouseInput(MouseConstants _input);
	// コントローラ列挙体を評価
	InputValue EvaluateGamePadInput(GamePadConstants _input);

	// InputCompositeTypeがAXIS2Dだった時の関数
	InputValue EvaluateAxis2d(const InputCompositeBindingData& _binding);

	// 現在の値を新しいものにする
	void MergeInputValue(InputValueType _valueType, const InputValue& _newValue, InputValue& _current, const float _threshold);

	// PRESSの評価関数
	InputInteractionEvents UpdateInteractionPress(InputActionRuntimeState& _state);
	// HOLDの評価関数
	InputInteractionEvents UpdateInteractionHold(InputActionRuntimeState& _state,float _duration);
	// TAPの評価関数
	InputInteractionEvents UpdateInteractionTap(InputActionRuntimeState& _state, float _duration);

	// 値を評価して真偽値を返す
	bool IsActuated(const InputValue& _value, const float _threshold = 0.0f) const;

	// 対応するコールバック関数をすべて呼ぶ
	void InvokePhaseCallbacks(InputActionRuntimeData& _runtimeData, const InputInteractionEvents& _events);
	void InvokeCallbacks(std::vector<InputCallbackEntry>& _callbacks, InputActionContext& _context);

	bool AddCallback(std::vector<InputCallbackEntry>& _pendingAddList, InputCallbackEntry::InputActionCallbackFunc _function, void* _userData);
	bool RemoveCallback(std::vector<InputCallbackEntry>& _callbacks, InputCallbackEntry::InputActionCallbackFunc _function, void* _userData);
private:
	std::unique_ptr<IInput> nativeInput;
	InputActionAssetData data;
	std::unordered_map<std::uint64_t, InputActionRuntimeData> runtimeDatas;
	// 追加待ち関数達
	std::unordered_map<std::uint64_t, InputActionCallbacks> pendingAddLists;
private:
	// 入力を受け入れるゲームパッドの番号
	std::size_t activeGamePadIndex{ 0 };
};
