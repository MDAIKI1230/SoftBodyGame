#pragma once

#include <concepts>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>

#include "FieldMacros.h"
#include "MDMath.h"
#include "PropertyTarget.h"

struct SceneDocument;
struct CollisionSettingDocument;

namespace InspectorFieldDrawFunc
{
	struct CollisionCategory
	{
	};

	/*
		一つのFieldを、ラベルと入力欄を含む一行として描画する。

		_document : Fieldを持つコンポーネントなどの実体。条件付き設定の評価にも使う。
		_scene    : 参照先候補となるObjectの一覧を持つScene。
		_target   : 編集対象のObject IDとComponent ID。fieldNameは_field.nameを使う。
		_field    : フィールド名、メンバへのポインタ、guiOptionsを持つ情報。

		値は_document.*_field.memberから取得し、型に対応するDrawValueへ渡す。
		RowSettingsとLayoutSettingsによる表示条件・行の開始終了、リセットもここで扱う。
		テーブルやコンポーネントの見出し、他のFieldの走査は呼び出し側が担当する。

		戻り値は値が変更された場合にtrue。非表示など、変更がなければfalse。
	*/
	template<class Document, class Owner, class Value, class... Options>
	bool DrawField(Document& _document, const SceneDocument& _scene, const PropertyTarget& _target, const MDReflection::FieldDescriptor<Owner, Value, Options...>& _field, const CollisionSettingDocument* _collisionSettings = nullptr);

	namespace Detail
	{
		/*
			型別の入力描画。行やテーブルの開始終了は行わない。
			_labelにはRowSettings.inputLabel、_optionsにはFieldのguiOptionsを渡す。
			各オーバーロードが必要な設定をロードして、対応するWidgetsを呼ぶ。
			戻り値は入力した値が変更されたかどうか。
		*/

		// boolはCheckbox。数値用テンプレートからは除外する。
		template<class... Options>
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target,const char* _label, bool& _value, const std::tuple<Options...>& _options);

		// 整数・float・doubleはInputNumber。参照指定のあるID値は参照入力へ切り替える。
		template<class Value, class... Options>
			requires ((std::integral<Value> && !std::same_as<Value, bool>) || std::same_as<Value, float> || std::same_as<Value, double>)
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Value& _value, const std::tuple<Options...>& _options);

		// Vector2は成分ごとの数値入力。範囲・速度・成分名を設定から取得する。
		template<class... Options>
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Vector2& _value, const std::tuple<Options...>& _options);

		// Vector3はInputVector3。Color指定があればRGBの数値欄とピッカーを使う。
		template<class... Options>
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target,const char* _label, Vector3& _value, const std::tuple<Options...>& _options);

		// Vector4はInputVector4。Color指定があればRGBAの数値欄とピッカーを使う。
		template<class... Options>
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Vector4& _value, const std::tuple<Options...>& _options);

		// 文字列はInputText。FilePath・DirectoryPath指定時はInputPathを使う。
		template<class... Options>
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, std::string& _value, const std::tuple<Options...>& _options);

		// Enumは設定に含まれる選択肢を使用する。Flags指定で複数のビットを選択する。
		template<class Value, class... Options>
			requires std::is_enum_v<Value>
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Value& _value, const std::tuple<Options...>& _options);

		/*
			vector・array・listなど、値を所有するコンテナはInputCollectionを使う。
			追加・削除・並び替えの設定を適用し、履歴はコンテナ全体で一件にする。
			要素の入力では履歴を重ねて記録しない。spanなどのビューは対象外。
		*/
		template<class Container, class... Options>
			requires (
			std::ranges::forward_range<Container>
			&& std::ranges::sized_range<Container>
			&& !std::ranges::view<Container>
			&& !std::same_as<Container, std::string>
			&& requires { typename Container::value_type; })
		bool DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Container& _value, const std::tuple<Options...>& _options);
	}
}

#include "InspectorFieldDrawFunc.inl"