#pragma once

#include <tuple>

#include "MDGuiOption.h"

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

namespace LoadSettingFunc
{
	// 左側の項目名
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::Label& _option);
	// 入力欄側の名前
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::InputLabel& _option);
	// 項目の説明文
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::Tooltip& _option);
	// 項目の単位名
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::Unit& _option);
	// 左側の項目名を非表示にする
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::HideLabel&);
	// 項目を読み取り専用にする
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::ReadOnly&);
	// 項目を非表示にする
	constexpr void ApplyOption(RowSettings& _settings, const MDGuiOption::Hidden&);
	// 操作可能な条件
	template<class Document, class Owner>
	constexpr void ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::EnabledBy<Owner>& _option);
	// 表示する条件
	template<class Document, class Owner>
	constexpr void ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::VisibleBy<Owner>& _option);
	// 読み取り専用にする条件
	template<class Document, class Owner>
	constexpr void ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::ReadOnlyBy<Owner>& _option);
	// 操作可能な条件
	template<class Document, class Owner, class Value>
	constexpr void ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::EnabledIf<Owner, Value>& _option);
	// 表示する条件
	template<class Document, class Owner, class Value>
	constexpr void ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::VisibleIf<Owner, Value>& _option);
	// 読み取り専用にする条件
	template<class Document, class Owner, class Value>
	constexpr void ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::ReadOnlyIf<Owner, Value>& _option);

	// 直前の入力欄と横並びにする
	constexpr void ApplyOption(LayoutSettings& _settings, const MDGuiOption::SameLine& _option);
	// 字下げ幅
	constexpr void ApplyOption(LayoutSettings& _settings, const MDGuiOption::Indent& _option);
	// 項目の前の縦の空白
	constexpr void ApplyOption(LayoutSettings& _settings, const MDGuiOption::Space& _option);
	// 項目の前の区切り線
	constexpr void ApplyOption(LayoutSettings& _settings, const MDGuiOption::Separator&);
	// 入力欄の幅
	constexpr void ApplyOption(LayoutSettings& _settings, const MDGuiOption::Width& _option);
	// 行の最小の高さ
	constexpr void ApplyOption(LayoutSettings& _settings, const MDGuiOption::RowHeight& _option);

	// 左右の列幅の比率
	constexpr void ApplyOption(TableSettings& _settings, const MDGuiOption::Columns& _option);
	// 左の列の固定幅
	constexpr void ApplyOption(TableSettings& _settings, const MDGuiOption::LabelWidth& _option);
	// セル内の余白
	constexpr void ApplyOption(TableSettings& _settings, const MDGuiOption::CellPadding& _option);
	// 列幅の変更を許可する
	constexpr void ApplyOption(TableSettings& _settings, const MDGuiOption::ResizableColumns&);
	// テーブルの境界線を表示する
	constexpr void ApplyOption(TableSettings& _settings, const MDGuiOption::TableBorders&);
	// 行の背景色を交互に変える
	constexpr void ApplyOption(TableSettings& _settings, const MDGuiOption::RowBackground&);

	// 連続する項目のグループ名
	constexpr void ApplyOption(GroupSettings& _settings, const MDGuiOption::Group& _option);
	// 折り畳まない見出し
	constexpr void ApplyOption(GroupSettings& _settings, const MDGuiOption::Header& _option);
	// 最初に表示したときの開閉状態
	constexpr void ApplyOption(GroupSettings& _settings, const MDGuiOption::DefaultOpen& _option);
	// 折り畳みを使用するか
	constexpr void ApplyOption(GroupSettings& _settings, const MDGuiOption::Foldout& _option);

	// 数値の最小値
	template<class Value, class OptionValue>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Min<OptionValue>& _option);
	// 数値の最大値
	template<class Value, class OptionValue>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Max<OptionValue>& _option);
	// 数値の最小値と最大値
	template<class Value, class OptionValue>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Range<OptionValue>& _option);
	// 数値のドラッグ速度
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Speed& _option);
	// 数値の増減操作の刻み幅
	template<class Value, class OptionValue>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Step<OptionValue>& _option);
	// 高速な増減操作の刻み幅
	template<class Value, class OptionValue>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::FastStep<OptionValue>& _option);
	// 数値の表示書式
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Format& _option);
	// スライダー入力を使用する
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Slider&);
	// ドラッグ入力を使用する
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Drag&);
	// 文字入力欄と増減ボタンを使用する
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Input&);
	// 値の変化を対数スケールにする
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Logarithmic&);
	// 直接入力した値も指定範囲に収める
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Clamp&);
	// 直接入力への切り替えを禁止する
	template<class Value>
	constexpr void ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::NoDirectInput&);

	// 文字列が空のときの入力例
	constexpr void ApplyOption(TextSettings& _settings, const MDGuiOption::Hint& _option);
	// 複数行入力と入力欄の行数
	constexpr void ApplyOption(TextSettings& _settings, const MDGuiOption::Multiline& _option);
	// 文字列の最大バイト数
	constexpr void ApplyOption(TextSettings& _settings, const MDGuiOption::MaxLength& _option);

	// ベクトルの成分名
	constexpr void ApplyOption(VectorSettings& _settings, const MDGuiOption::AxisLabels& _option);

	// 色入力を使用する
	constexpr void ApplyOption(ColorSettings& _settings, const MDGuiOption::Color&);
	// HDRの色入力を許可する
	constexpr void ApplyOption(ColorSettings& _settings, const MDGuiOption::HDR&);

	// 単一のEnumの選択肢
	template<class Value, std::size_t Count, class OptionValue>
	constexpr void ApplyOption(EnumSettings<Value, Count>& _settings, const MDGuiOption::EnumItem<OptionValue>& _option);
	// Enumの選択肢一覧。値の型と選択肢の数を一致させる
	template<class Value, std::size_t Count, class OptionValue, std::size_t OptionCount>
	constexpr void ApplyOption(EnumSettings<Value, Count>& _settings, const MDGuiOption::EnumItems<OptionValue, OptionCount>& _option);
	// Enumをビットフラグとして扱う
	template<class Value, std::size_t Count>
	constexpr void ApplyOption(EnumSettings<Value, Count>& _settings, const MDGuiOption::Flags&);

	// ファイル選択を使用する
	constexpr void ApplyOption(PathSettings& _settings, const MDGuiOption::FilePath&);
	// 選択したファイルの拡張子付きファイル名だけを保持する
	constexpr void ApplyOption(PathSettings& _settings, const MDGuiOption::FileNameOnly&);
	// フォルダ選択を使用する
	constexpr void ApplyOption(PathSettings& _settings, const MDGuiOption::DirectoryPath&);
	// ファイル選択の拡張子の条件
	constexpr void ApplyOption(PathSettings& _settings, const MDGuiOption::FileFilter& _option);
	// 現在のパスが空、またはファイル名だけを保持する場合に開く初期フォルダ
	constexpr void ApplyOption(PathSettings& _settings, const MDGuiOption::InitialDirectory& _option);

	// アセットの参照先の種類
	constexpr void ApplyOption(ReferenceSettings& _settings, const MDGuiOption::AssetType& _option);
	// オブジェクトの参照選択を使用する
	constexpr void ApplyOption(ReferenceSettings& _settings, const MDGuiOption::ObjectReference&);
	// コンポーネントの参照先の種類
	constexpr void ApplyOption(ReferenceSettings& _settings, const MDGuiOption::ComponentReference& _option);
	// 参照先を未設定にできるか
	constexpr void ApplyOption(ReferenceSettings& _settings, const MDGuiOption::AllowNull& _option);

	// 配列やリストの最小要素数
	constexpr void ApplyOption(CollectionSettings& _settings, const MDGuiOption::MinCount& _option);
	// 配列やリストの最大要素数
	constexpr void ApplyOption(CollectionSettings& _settings, const MDGuiOption::MaxCount& _option);
	// 配列やリストの並び替えを許可する
	constexpr void ApplyOption(CollectionSettings& _settings, const MDGuiOption::Reorderable&);
	// 配列やリストの要素数を固定する
	constexpr void ApplyOption(CollectionSettings& _settings, const MDGuiOption::FixedSize&);

	// 値をリセットする関数
	template<class Value, class OptionValue>
	constexpr void ApplyOption(ResetSettings<Value>& _settings, const MDGuiOption::ResetValue<OptionValue>& _option);

	/*
		Documentの条件を判定し、対応するオプションから設定を組み立てる
		表示と操作の条件はすべて満たした場合、読み取り専用の条件は一つでも満たした場合に有効になる
		同じ値を複数回指定した場合は後の指定を使い、他の設定構造体向けのオプションは読み飛ばす
	*/
	template<class Settings, class Document, class... Options>
	constexpr Settings Load(const Document& _document, const std::tuple<Options...>& _options);

	// Documentを使わずに設定を組み立てる。条件付きオプションが対象に含まれる場合はDocumentも渡す
	template<class Settings, class... Options>
	constexpr Settings Load(const std::tuple<Options...>& _options);
}

#include "LoadSettingFunc.inl"