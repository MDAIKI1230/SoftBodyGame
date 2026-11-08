#pragma once

#include <tuple>
#include <type_traits>
#include "MDGuiOption.h"

namespace MDReflection
{
	template<class Owner, class Value, class... Options>
	struct FieldDescriptor
	{
		const char* name;
		Value Owner::* member;
		std::tuple<Options...> guiOptions{};
	};

	template<class... Fields>
	struct DocumentDescriptor
	{
		const char* name;
		std::tuple<Fields...> fields;
	};

	template<class Document, class Function>
	void ForEachField(Document& _document, Function&& _function)
	{
		using DocumentType = std::remove_cvref_t<Document>;

		static constexpr auto info{ DocumentType::GetDocumentInfo() };

		std::apply(
			[&](const auto&... _fields)
			{
				(static_cast<void>(
					_function(
						_fields,
						_document.*(_fields.member)
					)
					), ...);
			},
			info.fields
		);
	}
}

#define MD_FIELD(Name, Member, ...) \
    MDReflection::FieldDescriptor{ (Name), &MDDocumentType::Member, std::make_tuple(__VA_ARGS__) }

#define MD_DOCUMENT(Type, Name, ...) \
    static constexpr auto GetDocumentInfo() \
    { \
        using MDDocumentType = Type; \
        return MDReflection::DocumentDescriptor{ \
            (Name), \
            std::make_tuple(__VA_ARGS__) \
        }; \
    }

#define MD_HAS_COMPONENT(...)\
	static constexpr auto GetComponentInfo() \
	{ \
		return std::make_tuple(__VA_ARGS__); \
	}

// 数値入力の最小値
#define MD_GUI_MIN(Value) \
    MDGuiOption::Min{ (Value) }

// 数値入力の最大値
#define MD_GUI_MAX(Value) \
    MDGuiOption::Max{ (Value) }

// 数値入力の最小値と最大値
#define MD_GUI_RANGE(Min, Max) \
    MDGuiOption::Range{ (Min), (Max) }

// ドラッグで値を変更するときの速度
#define MD_GUI_SPEED(...) \
    MDGuiOption::Speed{ __VA_ARGS__ }

// 数値を増減ボタンやキー操作で変更するときの刻み幅
#define MD_GUI_STEP(Value) \
    MDGuiOption::Step{ (Value) }

// 高速な増減操作をするときの刻み幅
#define MD_GUI_FAST_STEP(Value) \
    MDGuiOption::FastStep{ (Value) }

// 数値の表示書式
#define MD_GUI_FORMAT(...) \
    MDGuiOption::Format{ __VA_ARGS__ }

// 数値をスライダーで入力する
#define MD_GUI_SLIDER() \
    MDGuiOption::Slider{}

// 数値をドラッグで入力する
#define MD_GUI_DRAG() \
    MDGuiOption::Drag{}

// 数値を文字入力欄と増減ボタンで入力する
#define MD_GUI_INPUT() \
    MDGuiOption::Input{}

// スライダーやドラッグの値の変化を対数スケールにする
#define MD_GUI_LOGARITHMIC() \
    MDGuiOption::Logarithmic{}

// 直接入力した数値も、指定した最小値と最大値の範囲に収める
#define MD_GUI_CLAMP() \
    MDGuiOption::Clamp{}

// ドラッグやスライダーから文字入力への切り替えを禁止する
#define MD_GUI_NO_DIRECT_INPUT() \
    MDGuiOption::NoDirectInput{}

// 左側に表示する項目名。保存や履歴に使うフィールド名は変更しない
#define MD_GUI_LABEL(Value) \
    MDGuiOption::Label{ (Value) }

// 入力欄側に表示する名前。横並びのチェックボックスのX・Y・Zなどに使う
#define MD_GUI_INPUT_LABEL(Value) \
    MDGuiOption::InputLabel{ (Value) }

// 項目にマウスを重ねたときに表示する説明文
#define MD_GUI_TOOLTIP(Value) \
    MDGuiOption::Tooltip{ (Value) }

// 項目に表示する単位名。値の単位変換は行わない
#define MD_GUI_UNIT(Value) \
    MDGuiOption::Unit{ (Value) }

// 左側の項目名を非表示にする
#define MD_GUI_HIDE_LABEL() \
    MDGuiOption::HideLabel{}

// 値を表示するが、編集はできなくする
#define MD_GUI_READ_ONLY() \
    MDGuiOption::ReadOnly{}

// Inspectorに表示しない。保存対象からは外さない
#define MD_GUI_HIDDEN() \
    MDGuiOption::Hidden{}

// 指定したboolメンバがexpectedと一致するときだけ編集を有効にする
#define MD_GUI_ENABLED_BY(Member, ...) \
    MDGuiOption::EnabledBy{ (Member), __VA_ARGS__ }

// 指定したboolメンバがexpectedと一致するときだけ項目を表示する
#define MD_GUI_VISIBLE_BY(Member, ...) \
    MDGuiOption::VisibleBy{ (Member), __VA_ARGS__ }

// 指定したboolメンバがexpectedと一致するときだけ読み取り専用にする
#define MD_GUI_READ_ONLY_BY(Member, ...) \
    MDGuiOption::ReadOnlyBy{ (Member), __VA_ARGS__ }

// 指定したメンバがexpectedと一致するときだけ編集を有効にする
#define MD_GUI_ENABLED_IF(Member, Value) \
    MDGuiOption::EnabledIf{ (Member), (Value) }

// 指定したメンバがexpectedと一致するときだけ項目を表示する
#define MD_GUI_VISIBLE_IF(Member, Value) \
    MDGuiOption::VisibleIf{ (Member), (Value) }

// 指定したメンバがexpectedと一致するときだけ読み取り専用にする
#define MD_GUI_READ_ONLY_IF(Member, Value) \
    MDGuiOption::ReadOnlyIf{ (Member), (Value) }

// 同じ名前を指定した連続する項目を、折り畳みグループにまとめる
#define MD_GUI_GROUP(Name) \
    MDGuiOption::Group{ (Name) }

// 項目の前に表示する、折り畳まない見出し
#define MD_GUI_HEADER(Value) \
    MDGuiOption::Header{ (Value) }

// コンポーネントやグループを最初に表示したときの開閉状態
#define MD_GUI_DEFAULT_OPEN(...) \
    MDGuiOption::DefaultOpen{ __VA_ARGS__ }

// コンポーネントやグループを折り畳めるようにするか
#define MD_GUI_FOLDOUT(...) \
    MDGuiOption::Foldout{ __VA_ARGS__ }

// 新しい行を作らず、直前の入力欄と同じセル内で横に並べる
#define MD_GUI_SAME_LINE(...) \
    MDGuiOption::SameLine{ __VA_ARGS__ }

// 項目やグループの字下げ幅。0なら既定の字下げ幅を使う
#define MD_GUI_INDENT(...) \
    MDGuiOption::Indent{ __VA_ARGS__ }

// 項目の前に追加する縦の空白の高さ
#define MD_GUI_SPACE(...) \
    MDGuiOption::Space{ __VA_ARGS__ }

// 項目の前に区切り線を表示する
#define MD_GUI_SEPARATOR() \
    MDGuiOption::Separator{}

// 入力欄の幅。正数は固定幅、0は既定幅、負数は残り幅から差し引く幅
#define MD_GUI_WIDTH(...) \
    MDGuiOption::Width{ __VA_ARGS__ }

// テーブルの行の最小の高さ
#define MD_GUI_ROW_HEIGHT(...) \
    MDGuiOption::RowHeight{ __VA_ARGS__ }

// テーブルの左のラベル列と右の入力列の幅の比率
#define MD_GUI_COLUMNS(...) \
    MDGuiOption::Columns{ __VA_ARGS__ }

// テーブルの左のラベル列を固定幅で指定する
#define MD_GUI_LABEL_WIDTH(...) \
    MDGuiOption::LabelWidth{ __VA_ARGS__ }

// テーブルの各セルの横と縦の余白
#define MD_GUI_CELL_PADDING(...) \
    MDGuiOption::CellPadding{ __VA_ARGS__ }

// テーブルの列幅をマウスで変更できるようにする
#define MD_GUI_RESIZABLE_COLUMNS() \
    MDGuiOption::ResizableColumns{}

// テーブルの外枠と列や行の境界線を表示する
#define MD_GUI_TABLE_BORDERS() \
    MDGuiOption::TableBorders{}

// テーブルの行の背景色を交互に変える
#define MD_GUI_ROW_BACKGROUND() \
    MDGuiOption::RowBackground{}

// 文字列入力が空のときに表示する入力例
#define MD_GUI_HINT(Value) \
    MDGuiOption::Hint{ (Value) }

// 文字列を複数行で入力する。linesは入力欄に確保する行数
#define MD_GUI_MULTILINE(...) \
    MDGuiOption::Multiline{ __VA_ARGS__ }

// 文字列入力の最大バイト数。終端文字は含めない
#define MD_GUI_MAX_LENGTH(Value) \
    MDGuiOption::MaxLength{ (Value) }

// ベクトルの各成分に表示する名前
#define MD_GUI_AXIS_LABELS(...) \
    MDGuiOption::AxisLabels{ __VA_ARGS__ }

// 値をRGBまたはRGBAの色入力として表示する
#define MD_GUI_COLOR() \
    MDGuiOption::Color{}

// 色入力で、明るさが1を超えるHDRの値を扱えるようにする
#define MD_GUI_HDR() \
    MDGuiOption::HDR{}

// Enumの選択肢に表示する名前と、対応する値
#define MD_GUI_ENUM_ITEM(Label, Value) \
    MDGuiOption::EnumItem{ (Label), (Value) }

// Enumの選択肢一覧。選択肢のデータを配列として保持する
#define MD_GUI_ENUM_ITEMS(...) \
    MDGuiOption::EnumItems{ std::array{ __VA_ARGS__ } }

// Enumをビットフラグとして扱い、複数の項目を選択できるようにする
#define MD_GUI_FLAGS() \
    MDGuiOption::Flags{}

// 文字列をファイルパスとして扱い、ファイル選択を表示する
#define MD_GUI_FILE_PATH() \
    MDGuiOption::FilePath{}

// ファイルパス入力で、拡張子付きファイル名だけを保持する
#define MD_GUI_FILE_NAME_ONLY() \
	MDGuiOption::FileNameOnly{}

// 文字列をフォルダパスとして扱い、フォルダ選択を表示する
#define MD_GUI_DIRECTORY_PATH() \
    MDGuiOption::DirectoryPath{}

// ファイル選択で表示する拡張子の条件。複数ある場合は*.png;*.jpgのように指定する
#define MD_GUI_FILE_FILTER(Value) \
    MDGuiOption::FileFilter{ (Value) }

// 現在のパスが空、またはファイル名だけを保持する場合に開く初期フォルダ
#define MD_GUI_INITIAL_DIRECTORY(Value) \
	MDGuiOption::InitialDirectory{ (Value) }

// アセット選択で扱う種類名。ModelやTextureなどを指定する
#define MD_GUI_ASSET_TYPE(Value) \
    MDGuiOption::AssetType{ (Value) }

// 値をシーン内のオブジェクトへの参照として表示する
#define MD_GUI_OBJECT_REFERENCE() \
    MDGuiOption::ObjectReference{}

// 値をコンポーネントへの参照として表示する。typeNameは選択対象の種類名
#define MD_GUI_COMPONENT_REFERENCE(...) \
    MDGuiOption::ComponentReference{ __VA_ARGS__ }

// オブジェクトやアセットへの参照を未設定にできるか
#define MD_GUI_ALLOW_NULL(...) \
    MDGuiOption::AllowNull{ __VA_ARGS__ }

// 配列やリストの要素数の最小値
#define MD_GUI_MIN_COUNT(Value) \
    MDGuiOption::MinCount{ (Value) }

// 配列やリストの要素数の最大値
#define MD_GUI_MAX_COUNT(Value) \
    MDGuiOption::MaxCount{ (Value) }

// 配列やリストの要素の並び替えを有効にする
#define MD_GUI_REORDERABLE() \
    MDGuiOption::Reorderable{}

// 配列やリストの現在の要素数を固定し、追加と削除をできなくする
#define MD_GUI_FIXED_SIZE() \
    MDGuiOption::FixedSize{}

// リセット操作で書き戻す値を返す関数
#define MD_GUI_RESET_VALUE(GetValue) \
    MDGuiOption::ResetValue{ (GetValue) }