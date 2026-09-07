#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "SingleTonMacros.h"

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
	struct CallbackAccessKey
	{
		friend class InputAction;

	private:
		CallbackAccessKey() = default;
	};
public:
	// 入力のアダプターを入れる
	static void SetNativeInput(std::unique_ptr<IInput>&& _nativeInput)
	SingletonPublicFunction(void, SetNativeInput, (std::unique_ptr<IInput>&& _nativeInput), (std::move(_nativeInput)))
	// 初期化
	static void Initialize()
	SingletonPublicFunction(void, Initialize, (), ())
	// アセットの読み込み
	static bool LoadAsset(std::string _path)
	SingletonPublicFunction(bool, LoadAsset, (std::string _path), (_path))
	// 更新処理
	static void Update()
	SingletonPublicFunction(void, Update, (), ())
	// InputAction取得関数
	static InputAction GetInputAction(std::string_view _actionMapName, std::string_view _inputActionName)
	SingletonPublicFunction(InputAction, GetInputAction, (std::string_view _actionMapName, std::string_view _inputActionName), (_actionMapName, _inputActionName))


	// Startedの追加待ちに追加
	static bool QueueStartedCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
	SingletonPublicFunction(bool, QueueStartedCallbackAdd, (InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user), (_id, _func, _user))
	// Performedの追加待ちに追加
	static bool QueuePerformedCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
	SingletonPublicFunction(bool, QueuePerformedCallbackAdd, (InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user), (_id, _func, _user))
	// Canceledの追加待ちに追加
	static bool QueueCanceledCallbackAdd(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
	SingletonPublicFunction(bool, QueueCanceledCallbackAdd, (InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user), (_id, _func, _user))

	// Startedの解除待ちにする
	static bool StartedCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
	SingletonPublicFunction(bool, StartedCallbackRemove, (InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user), (_id, _func, _user))
	// Performedの解除待ちにする
	static bool PerformedCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
	SingletonPublicFunction(bool, PerformedCallbackRemove, (InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user), (_id, _func, _user))
	// Canceledの解除待ちにする
	static bool CanceledCallbackRemove(CallbackAccessKey, InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user)
	SingletonPublicFunction(bool, CanceledCallbackRemove, (InputDataID _id, InputCallbackEntry::InputActionCallbackFunc _func, void* _user), (_id, _func, _user))

private:
	// シングルトンにする
	InputSystem() = default;

	static InputSystem& Instance()
	{
		static InputSystem instance;
		return instance;
	}

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
