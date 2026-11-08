#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <utility>

#include <imgui.h>

#include "SingletonMacros.h"
#include "MDMath.h"
#include "CommandHistory.h"
#include "PropertyTarget.h"

#include "RowSettings.h"
#include "LayoutSettings.h"
#include "TableSettings.h"
#include "GroupSettings.h"
#include "NumberSettings.h"
#include "TextSettings.h"
#include "VectorSettings.h"
#include "ColorSettings.h"
#include "EnumSettings.h"
#include "PathSettings.h"
#include "ReferenceSettings.h"
#include "CollectionSettings.h"
#include "ResetSettings.h"

struct SceneDocument;

class CommandHistoryWidgets
{
public:
	// --- 開始終了 ---

	// 操作の無効化開始
	static void BeginDisabled(bool _disabled = true)
		SingletonPublicFunction(void, BeginDisabled, (bool _disabled), (_disabled))
	// 操作の無効化終了
	static void EndDisabled()
		SingletonPublicFunction(void, EndDisabled, (), ())
	// パネル開始
	static bool Begin(const char* _name, bool* _open = nullptr)
		SingletonPublicFunction(bool, Begin, (const char* _name, bool* _open), (_name, _open))
	// パネル終了
	static void End()
		SingletonPublicFunction(void, End, (), ())
	// テーブルの開始
	static bool BeginPropertyTable(const PropertyTarget& _target, const TableSettings& _settings = {})
		SingletonPublicFunction(bool, BeginPropertyTable, (const PropertyTarget& _target, const TableSettings& _settings), (_target, _settings))
	// テーブルの終了
	static void EndPropertyTable()
		SingletonPublicFunction(void, EndPropertyTable, (), ())
	// テーブル内の一行の開始。非表示の項目はfalseを返す
	static bool BeginPropertyRow(const PropertyTarget& _target, const char* _name, const RowSettings& _rowSettings = {}, const LayoutSettings& _layoutSettings = {})
		SingletonPublicFunction(bool, BeginPropertyRow, (const PropertyTarget& _target, const char* _name, const RowSettings& _rowSettings, const LayoutSettings& _layoutSettings), (_target, _name, _rowSettings, _layoutSettings))
	// テーブル内の一行の終了。開始時と同じ設定を渡す
	static void EndPropertyRow(const RowSettings& _rowSettings, const LayoutSettings& _layoutSettings)
		SingletonPublicFunction(void, EndPropertyRow, (const RowSettings& _rowSettings, const LayoutSettings& _layoutSettings), (_rowSettings, _layoutSettings))
	// メインメニューバー開始
	static bool BeginMainMenuBar()
		SingletonPublicFunction(bool, BeginMainMenuBar, (), ())
	// メインメニューバー終了
	static void EndMainMenuBar()
		SingletonPublicFunction(void, EndMainMenuBar, (), ())
	// メニュー開始
	static bool BeginMenu(const char* _label, bool _enabled = true)
		SingletonPublicFunction(bool, BeginMenu, (const char* _label, bool _enabled), (_label, _enabled))
	// メニュー終了
	static void EndMenu()
		SingletonPublicFunction(void, EndMenu, (), ())

	// --- 見出し系 ---

	struct HeaderResult
	{
		bool open{ true };
		bool clicked{ false };
		bool closed{ false };

		// 既存のboolでの利用では、これまでどおり開閉状態を返す。
		constexpr operator bool() const noexcept { return open; }
	};

	// 見出し描画。開閉状態、選択用のクリック、閉じる要求を別々に返す。
	static HeaderResult Header(const PropertyTarget& _target, const char* _label, const GroupSettings& _settings = {})
		SingletonPublicFunction(HeaderResult, Header, (const PropertyTarget& _target, const char* _label, const GroupSettings& _settings), (_target, _label, _settings))

	// ツリー行。openがtrueなら、子の描画後にTreePopを呼ぶ。
	static HeaderResult TreeNode(const PropertyTarget& _target, const char* _label, const GroupSettings& _settings = {})
		SingletonPublicFunction(HeaderResult, TreeNode, (const PropertyTarget& _target, const char* _label, const GroupSettings& _settings), (_target, _label, _settings))
	static void TreePop()
		SingletonPublicFunction(void, TreePop, (), ())

	// --- ドラッグ＆ドロップ ---

	// 行の直後に呼び、Object IDの値だけをドラッグ中のデータへコピーする。
	static void ObjectDragSource(std::uint64_t _objectID, const char* _label = nullptr)
		SingletonPublicFunction(void, ObjectDragSource, (std::uint64_t _objectID, const char* _label), (_objectID, _label))
	// 行の直後に呼ぶ。ドロップ確定時だけtrueを返し、移動元のIDを書き込む。
	static bool ObjectDropTarget(std::uint64_t& _objectID)
		SingletonPublicFunction(bool, ObjectDropTarget, (std::uint64_t& _objectID), (_objectID))
	// 余白用のドロップ領域。サイズの0成分は残りの描画領域を使う。
	static bool ObjectDropArea(const char* _id, std::uint64_t& _objectID, const Vector2& _size = Vector2{ 0.0f, 0.0f })
		SingletonPublicFunction(bool, ObjectDropArea, (const char* _id, std::uint64_t& _objectID, const Vector2& _size), (_id, _objectID, _size))
	// ツリーの描画後に呼ぶ。親変更を検証して適用し、履歴を1件記録する。
	static bool SetObjectParent(SceneDocument& _document, std::uint64_t _objectID, std::uint64_t _parentObjectID)
		SingletonPublicFunction(bool, SetObjectParent, (SceneDocument& _document, std::uint64_t _objectID, std::uint64_t _parentObjectID), (_document, _objectID, _parentObjectID))

	// --- 入力欄 ---

	// 数字入力の共通処理部分
	template<class T>
	static bool InputNumber(const PropertyTarget& _target, const char* _label, T& _value, const NumberSettings<T>& _settings = {})
		SingletonPublicTemplateFunction(bool, (class T), (T), InputNumber, (const PropertyTarget& _target, const char* _label, T& _value, const NumberSettings<T>& _settings), (_target, _label, _value, _settings))
	// Bool入力
	static bool Checkbox(const PropertyTarget& _target, const char* _label, bool& _value)
		SingletonPublicFunction(bool, Checkbox, (const PropertyTarget& _target, const char* _label, bool& _value), (_target, _label, _value))
	// 文字列入力。ヒントと複数行の指定もTextSettingsで扱う
	static bool InputText(const PropertyTarget& _target, const char* _label, std::string* _value, const TextSettings& _settings = {})
		SingletonPublicFunction(bool, InputText, (const PropertyTarget& _target, const char* _label, std::string* _value, const TextSettings& _settings), (_target, _label, _value, _settings))
	// 文字列の候補選択
	static bool InputChoice(const PropertyTarget& _target, const char* _label, std::string& _value, std::span<const std::string> _options)
		SingletonPublicFunction(bool, InputChoice, (const PropertyTarget& _target, const char* _label, std::string& _value, std::span<const std::string> _options), (_target, _label, _value, _options))
	// 2次元ベクトル入力
	static bool InputVector2(const PropertyTarget& _target, const char* _label, Vector2& _value, const FloatSettings& _numberSettings = {}, const VectorSettings& _vectorSettings = {})
		SingletonPublicFunction(bool, InputVector2, (const PropertyTarget& _target, const char* _label, Vector2& _value, const FloatSettings& _numberSettings, const VectorSettings& _vectorSettings), (_target, _label, _value, _numberSettings, _vectorSettings))
	// 3次元ベクトル入力
	static bool InputVector3(const PropertyTarget& _target, const char* _label, Vector3& _value, const FloatSettings& _numberSettings = {}, const VectorSettings& _vectorSettings = {})
		SingletonPublicFunction(bool, InputVector3, (const PropertyTarget& _target, const char* _label, Vector3& _value, const FloatSettings& _numberSettings, const VectorSettings& _vectorSettings), (_target, _label, _value, _numberSettings, _vectorSettings))
	// 4次元ベクトル入力
	static bool InputVector4(const PropertyTarget& _target, const char* _label, Vector4& _value, const FloatSettings& _numberSettings = {}, const VectorSettings& _vectorSettings = {})
		SingletonPublicFunction(bool, InputVector4, (const PropertyTarget& _target, const char* _label, Vector4& _value, const FloatSettings& _numberSettings, const VectorSettings& _vectorSettings), (_target, _label, _value, _numberSettings, _vectorSettings))
	// α抜きの色入力
	static bool InputColor3(const PropertyTarget& _target, const char* _label, Vector3& _value, const ColorSettings& _settings = {})
		SingletonPublicFunction(bool, InputColor3, (const PropertyTarget& _target, const char* _label, Vector3& _value, const ColorSettings& _settings), (_target, _label, _value, _settings))
	// α込みの色入力
	static bool InputColor4(const PropertyTarget& _target, const char* _label, Vector4& _value, const ColorSettings& _settings = {})
		SingletonPublicFunction(bool, InputColor4, (const PropertyTarget& _target, const char* _label, Vector4& _value, const ColorSettings& _settings), (_target, _label, _value, _settings))
	// 列挙体入力
	template<class T, std::size_t Count = 0>
	static bool InputEnum(const PropertyTarget& _target, const char* _label, T& _value, const EnumSettings<T, Count>& _settings = {})
		SingletonPublicTemplateFunction(
			bool,
			(class T, std::size_t Count),
			(T, Count),
			InputEnum,
			(const PropertyTarget& _target, const char* _label, T& _value, const EnumSettings<T, Count>& _settings),
			(_target, _label, _value, _settings))
	// ファイルパスの入力
	static bool InputPath(const PropertyTarget& _target, const char* _label, std::string* _value, const PathSettings& _pathSettings = {}, const TextSettings& _textSettings = {})
		SingletonPublicFunction(bool, InputPath, (const PropertyTarget& _target, const char* _label, std::string* _value, const PathSettings& _pathSettings, const TextSettings& _textSettings), (_target, _label, _value, _pathSettings, _textSettings))
	// オブジェクト参照入力。候補はSceneDocumentから取得する
	static bool InputReference(const PropertyTarget& _target, const char* _label, std::uint64_t& _objectID, const SceneDocument& _document, const ReferenceSettings& _settings = {})
		SingletonPublicFunction(bool, InputReference, (const PropertyTarget& _target, const char* _label, std::uint64_t& _objectID, const SceneDocument& _document, const ReferenceSettings& _settings), (_target, _label, _objectID, _document, _settings))
	// 配列の入力。履歴にはコンテナ全体を保存し、_drawElement(element, index)は履歴を持たない描画を行い、変更時にtrueを返す
	template<class Container, class DrawElement>
	static bool InputCollection(const PropertyTarget& _target, const char* _label, Container& _values, DrawElement&& _drawElement, const CollectionSettings& _settings = {})
		SingletonPublicTemplateFunction(
			bool,
			(class Container, class DrawElement),
			(Container, DrawElement),
			InputCollection,
			(const PropertyTarget& _target, const char* _label, Container& _values, DrawElement&& _drawElement, const CollectionSettings& _settings),
			(_target, _label, _values, std::forward<DrawElement>(_drawElement), _settings))
	// 値リセットボタン
	template<class T>
	static bool ResetButton(const PropertyTarget& _target, T& _value, const ResetSettings<T>& _settings = {})
		SingletonPublicTemplateFunction(bool, (class T), (T), ResetButton, (const PropertyTarget& _target, T& _value, const ResetSettings<T>& _settings), (_target, _value, _settings))

	// 検索用文字列入力(履歴には残さない)
	static bool SearchField(const char* _label, const char* _hint, std::string* _value)
		SingletonPublicFunction(bool, SearchField, (const char* _label, const char* _hint, std::string* _value), (_label, _hint, _value))

	// -- ちょっとしたオプション系 ---
	
	// ボタン
	static bool Button(const char* _label, const Vector2& _size = Vector2{ 0.0f, 0.0f })
		SingletonPublicFunction(bool, Button, (const char* _label, const Vector2& _size), (_label, _size))
	// 横並び
	static void SameLine(float _offset = 0.0f, float _spacing = -1.0f)
		SingletonPublicFunction(void, SameLine, (float _offset, float _spacing), (_offset, _spacing))
	// 区切り線
	static void Separator()
		SingletonPublicFunction(void, Separator, (), ())
	// 選択可能な項目
	static bool Selectable(const char* _label, bool _selected = false)
		SingletonPublicFunction(bool, Selectable, (const char* _label, bool _selected), (_label, _selected))

	// --- 純粋な描画 ---

	// テキスト表示
		static void Text(const char* _format, ...);
	// 画像表示
	static void Image(void* _texture, const Vector2& _size);

	// --- メニュー ---

	// メニュー項目
	static bool MenuItem(const char* _label, const char* _shortcut = nullptr, bool* _selected = nullptr, bool _enabled = true)
		SingletonPublicFunction(bool, MenuItem, (const char* _label, const char* _shortcut, bool* _selected, bool _enabled), (_label, _shortcut, _selected, _enabled))

	// ポップアップを開く
	static void OpenPopup(const char* _id)
		SingletonPublicFunction(void, OpenPopup, (const char* _id), (_id))
	// ポップアップ開始
	static bool BeginPopup(const char* _id)
		SingletonPublicFunction(bool, BeginPopup, (const char* _id), (_id))
	// ポップアップ終了
	static void EndPopup()
		SingletonPublicFunction(void, EndPopup, (), ())

	// --- 設定もしくは初期化 ---
	
	// ドッキング領域作成
	static std::uint32_t DockSpaceOverViewport()
		SingletonPublicFunction(std::uint32_t, DockSpaceOverViewport, (), ())
	// 利用可能な描画領域のサイズ取得
	static Vector2 GetContentRegionAvail()
		SingletonPublicFunction(Vector2, GetContentRegionAvail, (), ())
	// CommandHistoryを入れる
	static void SetCommandHistory(CommandHistory* _history)
		SingletonPublicFunction(void, SetCommandHistory, (CommandHistory* _history), (_history))

private:
	// 共通の履歴の残し処理。複合入力全体の操作状態もまとめて扱う
	template<class T, class Draw, class... Args>
	bool EditProperty(const PropertyTarget& _target, T& _value, Draw _draw, Args&&... _args);

	// 履歴を持たない数値描画。単独数値とベクトルの成分で共用する
	template<class T>
	bool DrawNumber(T& _value, const char* _label, const NumberSettings<T>& _settings);

	// 履歴処理から呼び出す、値ごとの入力描画
	bool DrawCheckbox(bool& _value, const char* _label);
	bool DrawTextInput(std::string& _value, const char* _label, const TextSettings& _settings);
	bool DrawPathInput(std::string& _value, const char* _label, const PathSettings& _pathSettings, const TextSettings& _textSettings);
	bool DrawVector2(Vector2& _value, const char* _label, const FloatSettings& _numberSettings, const VectorSettings& _vectorSettings);
	bool DrawVector3(Vector3& _value, const char* _label, const FloatSettings& _numberSettings, const VectorSettings& _vectorSettings);
	bool DrawVector4(Vector4& _value, const char* _label, const FloatSettings& _numberSettings, const VectorSettings& _vectorSettings);
	bool DrawColor3(Vector3& _value, const char* _label, const ColorSettings& _settings);
	bool DrawColor4(Vector4& _value, const char* _label, const ColorSettings& _settings);

	// Enumの選択肢と、ビットフラグの複数選択を描画する
	template<class T, std::size_t Count>
	bool DrawEnum(T& _value, const char* _label, const EnumSettings<T, Count>& _settings);
	template<class T, std::size_t Count>
	std::string MakeEnumPreview(const T& _value, const EnumSettings<T, Count>& _settings);
	template<class T>
	static bool IsEnumItemSelected(const T& _value, const T& _item, bool _flags);
	template<class T>
	static bool ApplyEnumSelection(T& _value, const T& _item, bool _flags);
	template<class T>
	static auto GetEnumBits(T _value);

	// 配列全体の描画。要素の入力と構造の変更を一つの履歴処理で扱う
	template<class Container, class DrawElement>
	bool DrawCollection(Container& _values, const char* _label, DrawElement& _drawElement, const CollectionSettings& _settings);
	template<class Value, class Reference, class DrawElement>
	bool DrawCollectionElement(Reference&& _element, std::size_t _index, DrawElement& _drawElement);

	// 一発で変更がされる履歴の残し処理
	template<class T>
	bool SetPropertyValue(const PropertyTarget& _target, T& _value, const T& _newValue);

	// 数値設定からImGuiのフラグ作成
	template<class T>
	int MakeNumberFlags(const NumberSettings<T>& _settings);

	// 数値型に対応するImGuiのデータ型
	template<class T>
	static constexpr int GetNumberDataType();

	// 設定された上下限をImGuiのSliderで扱えるか
	template<class T>
	static bool CanUseNumberSlider(const NumberSettings<T>& _settings);

	// 非有限値の確認や、設定に応じた範囲制限
	template<class T>
	bool ValidateNumber(T& _value, const T& _before, const NumberSettings<T>& _settings);

	// 設定とコンテナの機能から追加・削除の可否を判定する
	template<class Container>
	static bool CanAddCollectionElement(const Container& _values, const CollectionSettings& _settings);
	template<class Container>
	static bool CanRemoveCollectionElement(const Container& _values, const CollectionSettings& _settings);

	// 要素上限を見ながら追加
	template<class Container>
	bool AddCollectionElement(Container& _values, const CollectionSettings& _settings);

	// 要素下限を見ながら除外
	template<class Container>
	bool RemoveCollectionElement(Container& _values, std::size_t _index, const CollectionSettings& _settings);

	// 並び替え出来るなら並び替え
	template<class Container>
	bool MoveCollectionElement(Container& _values, std::size_t _from, std::size_t _to, const CollectionSettings& _settings);

	// プロパティの対象ごとにImGuiのIDを分ける
	void PushPropertyID(const PropertyTarget& _target);

	// 文字列の最大バイト数をUTF-8の文字境界で制限するコールバック
	static int LimitTextLength(ImGuiInputTextCallbackData* _data);

	// 設定に応じてファイルまたはフォルダを選択する
	bool SelectPath(const PathSettings& _settings, const std::string& _current, std::string& _selected);

	// 軸名と成分ごとの数値入力をまとめて描画する
	template<std::size_t Count>
	bool DrawVectorComponents(const char* _label, const std::array<float*, Count>& _components, const FloatSettings& _numberSettings, const VectorSettings& _settings);

	// Object参照の検索で大文字・小文字を区別せず比較する
	static bool EqualIgnoreCase(unsigned char _left, unsigned char _right);

	// RGB/RGBAの数値欄と色見本を表示するためのフラグ
	int MakeColorFlags(const ColorSettings& _settings);

	// 説明文の表示
	void Tooltip(const char* _text);

private:
	CommandHistoryWidgets() = default;

	static CommandHistoryWidgets& Instance()
	{
		static CommandHistoryWidgets instance;
		return instance;
	}

private:
	CommandHistory* history{ nullptr };
	std::string choiceSearch;
};

#include "CommandHistoryWidgets.inl"