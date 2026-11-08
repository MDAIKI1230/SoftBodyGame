#pragma once

#include "LoadSettingFunc.h"

namespace LoadSettingFunc::Detail
{
	// Documentを使わない読み込みを区別する
	struct NoDocument
	{
	};
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::Label& _option)
{
	_settings.label = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::InputLabel& _option)
{
	_settings.inputLabel = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::Tooltip& _option)
{
	_settings.tooltip = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::Unit& _option)
{
	_settings.unit = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::HideLabel&)
{
	_settings.hideLabel = true;
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::ReadOnly&)
{
	_settings.readOnly = true;
}

constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const MDGuiOption::Hidden&)
{
	_settings.visible = false;
}

template<class Document, class Owner>
constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::EnabledBy<Owner>& _option)
{
	_settings.enabled = _settings.enabled &&
		(_option.member != nullptr && _document.*(_option.member) == _option.expected);
}

template<class Document, class Owner>
constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::VisibleBy<Owner>& _option)
{
	_settings.visible = _settings.visible &&
		(_option.member != nullptr && _document.*(_option.member) == _option.expected);
}

template<class Document, class Owner>
constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::ReadOnlyBy<Owner>& _option)
{
	_settings.readOnly = _settings.readOnly ||
		(_option.member != nullptr && _document.*(_option.member) == _option.expected);
}

template<class Document, class Owner, class Value>
constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::EnabledIf<Owner, Value>& _option)
{
	_settings.enabled = _settings.enabled &&
		(_option.member != nullptr && _document.*(_option.member) == _option.expected);
}

template<class Document, class Owner, class Value>
constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::VisibleIf<Owner, Value>& _option)
{
	_settings.visible = _settings.visible &&
		(_option.member != nullptr && _document.*(_option.member) == _option.expected);
}

template<class Document, class Owner, class Value>
constexpr void LoadSettingFunc::ApplyOption(RowSettings& _settings, const Document& _document, const MDGuiOption::ReadOnlyIf<Owner, Value>& _option)
{
	_settings.readOnly = _settings.readOnly ||
		(_option.member != nullptr && _document.*(_option.member) == _option.expected);
}

constexpr void LoadSettingFunc::ApplyOption(LayoutSettings& _settings, const MDGuiOption::SameLine& _option)
{
	_settings.sameLine = true;
	_settings.offset = _option.offset;
	_settings.spacing = _option.spacing;
}

constexpr void LoadSettingFunc::ApplyOption(LayoutSettings& _settings, const MDGuiOption::Indent& _option)
{
	_settings.indent = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(LayoutSettings& _settings, const MDGuiOption::Space& _option)
{
	_settings.space = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(LayoutSettings& _settings, const MDGuiOption::Separator&)
{
	_settings.separator = true;
}

constexpr void LoadSettingFunc::ApplyOption(LayoutSettings& _settings, const MDGuiOption::Width& _option)
{
	_settings.width = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(LayoutSettings& _settings, const MDGuiOption::RowHeight& _option)
{
	_settings.rowHeight = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(TableSettings& _settings, const MDGuiOption::Columns& _option)
{
	_settings.labelWeight = _option.labelWeight;
	_settings.valueWeight = _option.valueWeight;
}

constexpr void LoadSettingFunc::ApplyOption(TableSettings& _settings, const MDGuiOption::LabelWidth& _option)
{
	_settings.labelWidth = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(TableSettings& _settings, const MDGuiOption::CellPadding& _option)
{
	_settings.cellPadding = CellPaddingSettings{ _option.x, _option.y };
}

constexpr void LoadSettingFunc::ApplyOption(TableSettings& _settings, const MDGuiOption::ResizableColumns&)
{
	_settings.resizableColumns = true;
}

constexpr void LoadSettingFunc::ApplyOption(TableSettings& _settings, const MDGuiOption::TableBorders&)
{
	_settings.borders = true;
}

constexpr void LoadSettingFunc::ApplyOption(TableSettings& _settings, const MDGuiOption::RowBackground&)
{
	_settings.rowBackground = true;
}

constexpr void LoadSettingFunc::ApplyOption(GroupSettings& _settings, const MDGuiOption::Group& _option)
{
	_settings.name = _option.name;
}

constexpr void LoadSettingFunc::ApplyOption(GroupSettings& _settings, const MDGuiOption::Header& _option)
{
	_settings.header = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(GroupSettings& _settings, const MDGuiOption::DefaultOpen& _option)
{
	_settings.defaultOpen = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(GroupSettings& _settings, const MDGuiOption::Foldout& _option)
{
	_settings.foldout = _option.value;
}

template<class Value, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Min<OptionValue>& _option)
{
	_settings.min = static_cast<Value>(_option.value);
}

template<class Value, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Max<OptionValue>& _option)
{
	_settings.max = static_cast<Value>(_option.value);
}

template<class Value, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Range<OptionValue>& _option)
{
	_settings.min = static_cast<Value>(_option.min);
	_settings.max = static_cast<Value>(_option.max);
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Speed& _option)
{
	_settings.speed = _option.value;
}

template<class Value, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Step<OptionValue>& _option)
{
	_settings.step = static_cast<Value>(_option.value);
}

template<class Value, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::FastStep<OptionValue>& _option)
{
	_settings.fastStep = static_cast<Value>(_option.value);
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Format& _option)
{
	_settings.format = _option.value;
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Slider&)
{
	_settings.mode = NumberSettings<Value>::Mode::Slider;
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Drag&)
{
	_settings.mode = NumberSettings<Value>::Mode::Drag;
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Input&)
{
	_settings.mode = NumberSettings<Value>::Mode::Input;
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Logarithmic&)
{
	_settings.logarithmic = true;
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::Clamp&)
{
	_settings.clamp = true;
}

template<class Value>
constexpr void LoadSettingFunc::ApplyOption(NumberSettings<Value>& _settings, const MDGuiOption::NoDirectInput&)
{
	_settings.directInput = false;
}

constexpr void LoadSettingFunc::ApplyOption(TextSettings& _settings, const MDGuiOption::Hint& _option)
{
	_settings.hint = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(TextSettings& _settings, const MDGuiOption::Multiline& _option)
{
	_settings.multiline = true;
	_settings.lines = _option.lines;
}

constexpr void LoadSettingFunc::ApplyOption(TextSettings& _settings, const MDGuiOption::MaxLength& _option)
{
	_settings.maxLength = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(VectorSettings& _settings, const MDGuiOption::AxisLabels& _option)
{
	_settings.axisLabels = { _option.x, _option.y, _option.z, _option.w };
}

constexpr void LoadSettingFunc::ApplyOption(ColorSettings& _settings, const MDGuiOption::Color&)
{
	_settings.enabled = true;
}

constexpr void LoadSettingFunc::ApplyOption(ColorSettings& _settings, const MDGuiOption::HDR&)
{
	_settings.hdr = true;
}

template<class Value, std::size_t Count, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(EnumSettings<Value, Count>& _settings, const MDGuiOption::EnumItem<OptionValue>& _option)
{
	static_assert(std::is_same_v<Value, OptionValue>, "EnumSettings and EnumItem must use the same value type.");
	static_assert(Count == 1, "A single EnumItem requires EnumSettings<Value, 1>.");
	
	if constexpr (std::is_same_v<Value, OptionValue> && Count == 1)
	{
		_settings.items[0] = _option;
	}
}

template<class Value, std::size_t Count, class OptionValue, std::size_t OptionCount>
constexpr void LoadSettingFunc::ApplyOption(EnumSettings<Value, Count>& _settings, const MDGuiOption::EnumItems<OptionValue, OptionCount>& _option)
{
	static_assert(std::is_same_v<Value, OptionValue>, "EnumSettings and EnumItems must use the same value type.");
	static_assert(Count == OptionCount, "EnumSettings and EnumItems must use the same item count.");
	
	if constexpr (std::is_same_v<Value, OptionValue> && Count == OptionCount)
	{
		_settings.items = _option.values;
	}
}

template<class Value, std::size_t Count>
constexpr void LoadSettingFunc::ApplyOption(EnumSettings<Value, Count>& _settings, const MDGuiOption::Flags&)
{
	_settings.flags = true;
}

constexpr void LoadSettingFunc::ApplyOption(PathSettings& _settings, const MDGuiOption::FilePath&)
{
	_settings.kind = PathSettings::Kind::File;
}

constexpr void LoadSettingFunc::ApplyOption(PathSettings& _settings, const MDGuiOption::FileNameOnly&)
{
	_settings.fileNameOnly = true;
}

constexpr void LoadSettingFunc::ApplyOption(PathSettings& _settings, const MDGuiOption::DirectoryPath&)
{
	_settings.kind = PathSettings::Kind::Directory;
}

constexpr void LoadSettingFunc::ApplyOption(PathSettings& _settings, const MDGuiOption::FileFilter& _option)
{
	_settings.filter = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(PathSettings& _settings, const MDGuiOption::InitialDirectory& _option)
{
	_settings.initialDirectory = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(ReferenceSettings& _settings, const MDGuiOption::AssetType& _option)
{
	_settings.kind = ReferenceSettings::Kind::Asset;
	_settings.typeName = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(ReferenceSettings& _settings, const MDGuiOption::ObjectReference&)
{
	_settings.kind = ReferenceSettings::Kind::Object;
	_settings.typeName = nullptr;
}

constexpr void LoadSettingFunc::ApplyOption(ReferenceSettings& _settings, const MDGuiOption::ComponentReference& _option)
{
	_settings.kind = ReferenceSettings::Kind::Component;
	_settings.typeName = _option.typeName;
}

constexpr void LoadSettingFunc::ApplyOption(ReferenceSettings& _settings, const MDGuiOption::AllowNull& _option)
{
	_settings.allowNull = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(CollectionSettings& _settings, const MDGuiOption::MinCount& _option)
{
	_settings.minCount = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(CollectionSettings& _settings, const MDGuiOption::MaxCount& _option)
{
	_settings.maxCount = _option.value;
}

constexpr void LoadSettingFunc::ApplyOption(CollectionSettings& _settings, const MDGuiOption::Reorderable&)
{
	_settings.reorderable = true;
}

constexpr void LoadSettingFunc::ApplyOption(CollectionSettings& _settings, const MDGuiOption::FixedSize&)
{
	_settings.fixedSize = true;
}

template<class Value, class OptionValue>
constexpr void LoadSettingFunc::ApplyOption(ResetSettings<Value>& _settings, const MDGuiOption::ResetValue<OptionValue>& _option)
{
	static_assert(std::is_same_v<Value, OptionValue>, "ResetSettings and ResetValue must use the same value type.");
	
	if constexpr (std::is_same_v<Value, OptionValue>)
	{
		_settings.getValue = _option.getValue;
	}
}

template<class Settings, class Document, class... Options>
constexpr Settings LoadSettingFunc::Load(const Document& _document, const std::tuple<Options...>& _options)
{
	Settings result{};

	auto applyOption = [&](const auto& _option)
	{
		// Documentを使うオーバーロードを優先して、条件を判定する
		if constexpr (requires { ApplyOption(result, _document, _option); })
		{
			if constexpr (std::is_same_v<Document, Detail::NoDocument>)
			{
				static_assert(!std::is_same_v<Document, Detail::NoDocument>,
					"Conditional options require Load<Settings>(document, options).");
			}
			else
			{
				ApplyOption(result, _document, _option);
			}
		}
		// この設定構造体が扱うオプションだけを読み込む
		else if constexpr (requires { ApplyOption(result, _option); })
		{
			ApplyOption(result, _option);
		}
	};

	std::apply(
		[&](const auto&... _items)
		{
			(applyOption(_items), ...);
		},
		_options
	);

	return result;
}

template<class Settings, class... Options>
constexpr Settings LoadSettingFunc::Load(const std::tuple<Options...>& _options)
{
	return Load<Settings>(Detail::NoDocument{}, _options);
}