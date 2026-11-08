#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <utility>

#include <imgui.h>

#include "CommandHistoryWidgets.h"
#include "LoadSettingFunc.h"
#include "InspectorFieldDrawFunc.h"

namespace InspectorFieldDrawFunc::Detail
{
	inline bool DrawReference(const PropertyTarget& _target, const char* _label, std::uint64_t& _value, const SceneDocument& _document, const ReferenceSettings& _settings);
	inline void ReserveResetButton();
	inline void PlaceResetButton();

	// オプションの型から選択肢の数だけを取得する。
	template<class Option>
	constexpr std::size_t GetEnumOptionCount(const Option*);
	template<class Value>
	constexpr std::size_t GetEnumOptionCount(const MDGuiOption::EnumItem<Value>*);
	template<class Value, std::size_t Count>
	constexpr std::size_t GetEnumOptionCount(const MDGuiOption::EnumItems<Value, Count>*);

	template<class Value, class... Options, std::size_t... Indices>
	constexpr auto LoadEnumSettings(const std::tuple<Options...>& _options, std::index_sequence<Indices...> _indices);

	template<class Value, std::size_t Count, class Option>
	constexpr void AppendEnumOption(EnumSettings<Value, Count>& _settings, std::size_t& _index, const Option& _option);
	template<class Value, std::size_t Count, class OptionValue>
	constexpr void AppendEnumOption(EnumSettings<Value, Count>& _settings, std::size_t& _index, const MDGuiOption::EnumItem<OptionValue>& _option);
	template<class Value, std::size_t Count, class OptionValue, std::size_t OptionCount>
	constexpr void AppendEnumOption(EnumSettings<Value, Count>& _settings, std::size_t& _index, const MDGuiOption::EnumItems<OptionValue, OptionCount>& _option);

	// 要素の描画だけを担当する。設定は呼び出し時に渡し、履歴は親の配列で管理する。
	template<class Value, class... Options>
	bool DrawCollectionElement(const SceneDocument& _scene, const std::tuple<Options...>& _options, Value& _value, std::size_t _index);
}

template<class Document, class Owner, class Value, class... Options>
bool InspectorFieldDrawFunc::DrawField(Document& _document, const SceneDocument& _scene, const PropertyTarget& _target, const MDReflection::FieldDescriptor<Owner, Value, Options...>& _field)
{
	const auto row{ LoadSettingFunc::Load<RowSettings>(_document, _field.guiOptions) };
	const auto layout{ LoadSettingFunc::Load<LayoutSettings>(_field.guiOptions) };
	PropertyTarget target{ _target };
	target.fieldName = _field.name;

	if (!CommandHistoryWidgets::BeginPropertyRow(target, _field.name, row, layout))
	{
		return false;
	}

	auto& value{ _document.*_field.member };
	const auto reset{ LoadSettingFunc::Load<ResetSettings<Value>>(_field.guiOptions) };
	if (reset.getValue)
	{
		Detail::ReserveResetButton();
	}
	bool changed{ Detail::DrawValue(_scene, target, row.inputLabel ? row.inputLabel : "##Value", value, _field.guiOptions) };
	if (reset.getValue)
	{
		Detail::PlaceResetButton();
		changed |= CommandHistoryWidgets::ResetButton(target, value, reset);
	}
	CommandHistoryWidgets::EndPropertyRow(row, layout);
	return changed;
}

template<class... Options>
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target,const char* _label, bool& _value, const std::tuple<Options...>& _options)
{
	return CommandHistoryWidgets::Checkbox(_target, _label, _value);
}

template<class Value, class... Options>
	requires ((std::integral<Value> && !std::same_as<Value, bool>) || std::same_as<Value, float> || std::same_as<Value, double>)
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Value& _value, const std::tuple<Options...>& _options)
{
	const auto number{ LoadSettingFunc::Load<NumberSettings<Value>>(_options) };
	const auto reference{ LoadSettingFunc::Load<ReferenceSettings>(_options) };

	if constexpr (std::same_as<Value, std::uint64_t>)
	{
		if (reference.kind == ReferenceSettings::Kind::Object)
		{
			return DrawReference(_target, _label, _value, _scene, reference);
		}
	}

	if (reference.kind != ReferenceSettings::Kind::None)
	{
		// 参照指定のIDを数値として直接編集させない。
		CommandHistoryWidgets::BeginDisabled();
		CommandHistoryWidgets::InputNumber(_target, _label, _value, number);
		CommandHistoryWidgets::EndDisabled();
		return false;
	}

	return CommandHistoryWidgets::InputNumber(_target, _label, _value, number);
}

template<class... Options>
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Vector2& _value, const std::tuple<Options...>& _options)
{
	const auto number{ LoadSettingFunc::Load<FloatSettings>(_options) };
	const auto vector{ LoadSettingFunc::Load<VectorSettings>(_options) };
	return CommandHistoryWidgets::InputVector2(_target, _label, _value, number, vector);
}

template<class... Options>
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target,const char* _label, Vector3& _value, const std::tuple<Options...>& _options)
{
	const auto color{ LoadSettingFunc::Load<ColorSettings>(_options) };
	if (color.enabled)
	{
		return CommandHistoryWidgets::InputColor3(_target, _label, _value, color);
	}
	const auto number{ LoadSettingFunc::Load<FloatSettings>(_options) };
	const auto vector{ LoadSettingFunc::Load<VectorSettings>(_options) };
	return CommandHistoryWidgets::InputVector3(_target, _label, _value, number, vector);
}

template<class... Options>
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Vector4& _value, const std::tuple<Options...>& _options)
{
	const auto color{ LoadSettingFunc::Load<ColorSettings>(_options) };
	if (color.enabled)
	{
		return CommandHistoryWidgets::InputColor4(_target, _label, _value, color);
	}
	const auto number{ LoadSettingFunc::Load<FloatSettings>(_options) };
	const auto vector{ LoadSettingFunc::Load<VectorSettings>(_options) };
	return CommandHistoryWidgets::InputVector4(_target, _label, _value, number, vector);
}

template<class... Options>
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, std::string& _value, const std::tuple<Options...>& _options)
{
	const auto text{ LoadSettingFunc::Load<TextSettings>(_options) };
	const auto path{ LoadSettingFunc::Load<PathSettings>(_options) };
	if (path.kind != PathSettings::Kind::None)
	{
		return CommandHistoryWidgets::InputPath(_target, _label, &_value, path, text);
	}
	return CommandHistoryWidgets::InputText(_target, _label, &_value, text);
}

template<class Value, class... Options>
	requires std::is_enum_v<Value>
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Value& _value, const std::tuple<Options...>& _options)
{
	const auto settings{ LoadEnumSettings<Value>(_options, std::index_sequence_for<Options...>{}) };
	return CommandHistoryWidgets::InputEnum(_target, _label, _value, settings);
}

template<class Container, class... Options>
	requires (
	std::ranges::forward_range<Container>
	&& std::ranges::sized_range<Container>
	&& !std::ranges::view<Container>
	&& !std::same_as<Container, std::string>
	&& requires { typename Container::value_type; })
bool InspectorFieldDrawFunc::Detail::DrawValue(const SceneDocument& _scene, const PropertyTarget& _target, const char* _label, Container& _value, const std::tuple<Options...>& _options)
{
	const auto settings{ LoadSettingFunc::Load<CollectionSettings>(_options) };
	auto drawElement{ std::bind_front(&DrawCollectionElement<typename Container::value_type, Options...>, std::cref(_scene), std::cref(_options)) };
	return CommandHistoryWidgets::InputCollection(_target, _label, _value, drawElement, settings);
}

// --- 内部処理 ---

template<class Option>
constexpr std::size_t InspectorFieldDrawFunc::Detail::GetEnumOptionCount(const Option*)
{
	return 0;
}

template<class Value>
constexpr std::size_t InspectorFieldDrawFunc::Detail::GetEnumOptionCount(const MDGuiOption::EnumItem<Value>*)
{
	return 1;
}

template<class Value, std::size_t Count>
constexpr std::size_t InspectorFieldDrawFunc::Detail::GetEnumOptionCount(const MDGuiOption::EnumItems<Value, Count>*)
{
	return Count;
}

template<class Value, class... Options, std::size_t... Indices>
constexpr auto InspectorFieldDrawFunc::Detail::LoadEnumSettings(const std::tuple<Options...>& _options, std::index_sequence<Indices...> _indices)
{
	constexpr std::size_t count{ (GetEnumOptionCount(static_cast<const Options*>(nullptr)) + ... + 0) };
	EnumSettings<Value, count> settings;
	std::size_t index{ 0 };
	(AppendEnumOption(settings, index, std::get<Indices>(_options)), ...);
	return settings;
}

template<class Value, std::size_t Count, class Option>
constexpr void InspectorFieldDrawFunc::Detail::AppendEnumOption(EnumSettings<Value, Count>& _settings, std::size_t& _index, const Option& _option)
{
	// 選択肢以外の設定はLoadへ任せる。Flagsもここで適用する。
	_settings.flags |= LoadSettingFunc::Load<EnumSettings<Value>>(std::tie(_option)).flags;
}

template<class Value, std::size_t Count, class OptionValue>
constexpr void InspectorFieldDrawFunc::Detail::AppendEnumOption(EnumSettings<Value, Count>& _settings, std::size_t& _index, const MDGuiOption::EnumItem<OptionValue>& _option)
{
	const auto loaded{ LoadSettingFunc::Load<EnumSettings<Value, 1>>(std::tie(_option)) };
	_settings.items[_index++] = loaded.items[0];
}

template<class Value, std::size_t Count, class OptionValue, std::size_t OptionCount>
constexpr void InspectorFieldDrawFunc::Detail::AppendEnumOption(EnumSettings<Value, Count>& _settings, std::size_t& _index, const MDGuiOption::EnumItems<OptionValue, OptionCount>& _option)
{
	const auto loaded{ LoadSettingFunc::Load<EnumSettings<Value, OptionCount>>(std::tie(_option)) };
	for (const auto& item : loaded.items)
	{
		_settings.items[_index++] = item;
	}
}

template<class Value, class... Options>
bool InspectorFieldDrawFunc::Detail::DrawCollectionElement(const SceneDocument& _scene, const std::tuple<Options...>& _options, Value& _value, std::size_t _index)
{
	// 要素番号のIDと入力幅はInputCollectionが設定する。
	// ID=0はResolvePropertyで解決できないため、要素単体の履歴は作られない。
	// 親のInputCollectionだけが、実際のtargetへコンテナ全体の履歴を残す。
	const PropertyTarget elementTarget{};

	if constexpr (requires { Value::GetDocumentInfo(); })
	{
		if (!CommandHistoryWidgets::BeginPropertyTable(elementTarget))
		{
			return false;
		}

		bool changed{ false };

		MDReflection::ForEachField(
			_value,
			[&](const auto& field, auto&)
			{
				changed |= InspectorFieldDrawFunc::DrawField(
					_value, _scene, elementTarget, field);
			}
		);

		CommandHistoryWidgets::EndPropertyTable();
		return changed;
	}
	else
	{
		return DrawValue(_scene, elementTarget, "##Value", _value, _options);
	}
}


inline bool InspectorFieldDrawFunc::Detail::DrawReference(const PropertyTarget& _target, const char* _label, std::uint64_t& _value, const SceneDocument& _document, const ReferenceSettings& _settings)
{
	return CommandHistoryWidgets::InputReference(_target, _label, _value, _document, _settings);
}

inline void InspectorFieldDrawFunc::Detail::ReserveResetButton()
{
	const auto& style{ ImGui::GetStyle() };
	const float width{ ImGui::CalcTextSize("Reset").x + style.FramePadding.x * 2.0f };
	ImGui::SetNextItemWidth((std::max)(1.0f, ImGui::CalcItemWidth() - width - style.ItemInnerSpacing.x));
}

inline void InspectorFieldDrawFunc::Detail::PlaceResetButton()
{
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
}