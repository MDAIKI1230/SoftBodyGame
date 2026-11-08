#pragma once

#include <array>
#include <cstddef>

namespace MDGuiOption
{
	// 数値入力の最小値
	template<class Value>
	struct Min
	{
		Value value{};
	};

	// 数値入力の最大値
	template<class Value>
	struct Max
	{
		Value value{};
	};

	// 数値入力の最小値と最大値
	template<class Value>
	struct Range
	{
		Value min{};
		Value max{};
	};

	// ドラッグで値を変更するときの速度
	struct Speed
	{
		float value{ 0.1f };
	};

	// 数値を増減ボタンやキー操作で変更するときの刻み幅
	template<class Value>
	struct Step
	{
		Value value{};
	};

	// 高速な増減操作をするときの刻み幅
	template<class Value>
	struct FastStep
	{
		Value value{};
	};

	// 数値の表示書式
	struct Format
	{
		const char* value{ "%.3f" };
	};

	// 数値をスライダーで入力する
	struct Slider
	{
	};

	// 数値をドラッグで入力する
	struct Drag
	{
	};

	// 数値を文字入力欄と増減ボタンで入力する
	struct Input
	{
	};

	// スライダーやドラッグの値の変化を対数スケールにする
	struct Logarithmic
	{
	};

	// 直接入力した数値も、指定した最小値と最大値の範囲に収める
	struct Clamp
	{
	};

	// ドラッグやスライダーから文字入力への切り替えを禁止する
	struct NoDirectInput
	{
	};

	// 左側に表示する項目名。保存や履歴に使うフィールド名は変更しない
	struct Label
	{
		const char* value{ nullptr };
	};

	// 入力欄側に表示する名前。横並びのチェックボックスのX・Y・Zなどに使う
	struct InputLabel
	{
		const char* value{ nullptr };
	};

	// 項目にマウスを重ねたときに表示する説明文
	struct Tooltip
	{
		const char* value{ nullptr };
	};

	// 項目に表示する単位名。値の単位変換は行わない
	struct Unit
	{
		const char* value{ nullptr };
	};

	// 左側の項目名を非表示にする
	struct HideLabel
	{
	};

	// 値を表示するが、編集はできなくする
	struct ReadOnly
	{
	};

	// Inspectorに表示しない。保存対象からは外さない
	struct Hidden
	{
	};

	// 指定したboolメンバがexpectedと一致するときだけ編集を有効にする
	template<class Owner>
	struct EnabledBy
	{
		bool Owner::* member{ nullptr };
		bool expected{ true };
	};

	// 指定したboolメンバがexpectedと一致するときだけ項目を表示する
	template<class Owner>
	struct VisibleBy
	{
		bool Owner::* member{ nullptr };
		bool expected{ true };
	};

	// 指定したboolメンバがexpectedと一致するときだけ読み取り専用にする
	template<class Owner>
	struct ReadOnlyBy
	{
		bool Owner::* member{ nullptr };
		bool expected{ true };
	};

	// 指定したメンバがexpectedと一致するときだけ編集を有効にする
	template<class Owner, class Value>
	struct EnabledIf
	{
		Value Owner::* member{ nullptr };
		Value expected{};
	};

	// 指定したメンバがexpectedと一致するときだけ項目を表示する
	template<class Owner, class Value>
	struct VisibleIf
	{
		Value Owner::* member{ nullptr };
		Value expected{};
	};

	// 指定したメンバがexpectedと一致するときだけ読み取り専用にする
	template<class Owner, class Value>
	struct ReadOnlyIf
	{
		Value Owner::* member{ nullptr };
		Value expected{};
	};

	// 同じ名前を指定した連続する項目を、折り畳みグループにまとめる
	struct Group
	{
		const char* name{ nullptr };
	};

	// 項目の前に表示する、折り畳まない見出し
	struct Header
	{
		const char* value{ nullptr };
	};

	// コンポーネントやグループを最初に表示したときの開閉状態
	struct DefaultOpen
	{
		bool value{ true };
	};

	// コンポーネントやグループを折り畳めるようにするか
	struct Foldout
	{
		bool value{ true };
	};

	// 新しい行を作らず、直前の入力欄と同じセル内で横に並べる
	struct SameLine
	{
		// 行内の開始位置からの横位置。0なら直前の項目の隣に置く
		float offset{ 0.0f };
		// 直前の項目との間隔。負数なら既定の間隔を使う
		float spacing{ -1.0f };
	};

	// 項目やグループの字下げ幅。0なら既定の字下げ幅を使う
	struct Indent
	{
		float value{ 0.0f };
	};

	// 項目の前に追加する縦の空白の高さ
	struct Space
	{
		float value{ 0.0f };
	};

	// 項目の前に区切り線を表示する
	struct Separator
	{
	};

	// 入力欄の幅。正数は固定幅、0は既定幅、負数は残り幅から差し引く幅
	struct Width
	{
		float value{ 0.0f };
	};

	// テーブルの行の最小の高さ
	struct RowHeight
	{
		float value{ 0.0f };
	};

	// テーブルの左のラベル列と右の入力列の幅の比率
	struct Columns
	{
		float labelWeight{ 0.4f };
		float valueWeight{ 0.6f };
	};

	// テーブルの左のラベル列を固定幅で指定する
	struct LabelWidth
	{
		float value{ 0.0f };
	};

	// テーブルの各セルの横と縦の余白
	struct CellPadding
	{
		float x{ 4.0f };
		float y{ 2.0f };
	};

	// テーブルの列幅をマウスで変更できるようにする
	struct ResizableColumns
	{
	};

	// テーブルの外枠と列や行の境界線を表示する
	struct TableBorders
	{
	};

	// テーブルの行の背景色を交互に変える
	struct RowBackground
	{
	};

	// 文字列入力が空のときに表示する入力例
	struct Hint
	{
		const char* value{ nullptr };
	};

	// 文字列を複数行で入力する。linesは入力欄に確保する行数
	struct Multiline
	{
		int lines{ 4 };
	};

	// 文字列入力の最大バイト数。終端文字は含めない
	struct MaxLength
	{
		std::size_t value{};
	};

	// ベクトルの各成分に表示する名前
	struct AxisLabels
	{
		const char* x{ "X" };
		const char* y{ "Y" };
		const char* z{ "Z" };
		const char* w{ "W" };
	};

	// 値をRGBまたはRGBAの色入力として表示する
	struct Color
	{
	};

	// 色入力で、明るさが1を超えるHDRの値を扱えるようにする
	struct HDR
	{
	};

	// Enumの選択肢に表示する名前と、対応する値
	template<class Value>
	struct EnumItem
	{
		const char* label{ nullptr };
		Value value{};
	};

	// Enumの選択肢一覧。選択肢のデータを配列として保持する
	template<class Value, std::size_t Count>
	struct EnumItems
	{
		std::array<EnumItem<Value>, Count> values{};
	};

	// Enumをビットフラグとして扱い、複数の項目を選択できるようにする
	struct Flags
	{
	};

	// 文字列をファイルパスとして扱い、ファイル選択を表示する
	struct FilePath
	{
	};

	// 選択したファイルの拡張子付きファイル名だけを保持する
	struct FileNameOnly
	{
	};

	// 文字列をフォルダパスとして扱い、フォルダ選択を表示する
	struct DirectoryPath
	{
	};

	// ファイル選択で表示する拡張子の条件。複数ある場合は*.png;*.jpgのように指定する
	struct FileFilter
	{
		const char* value{ nullptr };
	};

	// 現在のパスが空、またはファイル名だけを保持する場合に開く初期フォルダ
	struct InitialDirectory
	{
		const char* value{ nullptr };
	};

	// アセット選択で扱う種類名。ModelやTextureなどを指定する
	struct AssetType
	{
		const char* value{ nullptr };
	};

	// 値をシーン内のオブジェクトへの参照として表示する
	struct ObjectReference
	{
	};

	// 値をコンポーネントへの参照として表示する。typeNameは選択対象の種類名
	struct ComponentReference
	{
		const char* typeName{ nullptr };
	};

	// オブジェクトやアセットへの参照を未設定にできるか
	struct AllowNull
	{
		bool value{ true };
	};

	// 配列やリストの要素数の最小値
	struct MinCount
	{
		std::size_t value{};
	};

	// 配列やリストの要素数の最大値
	struct MaxCount
	{
		std::size_t value{};
	};

	// 配列やリストの要素の並び替えを有効にする
	struct Reorderable
	{
	};

	// 配列やリストの現在の要素数を固定し、追加と削除をできなくする
	struct FixedSize
	{
	};

	// リセット操作で書き戻す値を返す関数
	template<class Value>
	struct ResetValue
	{
		Value (*getValue)(){ nullptr };
	};
}