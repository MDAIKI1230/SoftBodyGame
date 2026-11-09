#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <ranges>
#include <limits>
#include <type_traits>
#include <string_view>

#include <imgui.h>

#include "CommandHistoryWidgets.h"

// --- 入力欄 ---

template<class T>
bool CommandHistoryWidgets::InputNumberImpl(const PropertyTarget& _target, const char* _label, T& _value, const NumberSettings<T>& _settings)
{
	return EditProperty(_target, _value, &CommandHistoryWidgets::DrawNumber<T>, _label, _settings);
}

template<class T, std::size_t Count>
bool CommandHistoryWidgets::InputEnumImpl(const PropertyTarget& _target, const char* _label, T& _value, const EnumSettings<T, Count>& _settings)
{
	return EditProperty(_target, _value, &CommandHistoryWidgets::DrawEnum<T>, _label,
		std::span<const MDGuiOption::EnumItem<T>>{ _settings.items }, _settings.flags);
}

template<class Container, class DrawElement>
bool CommandHistoryWidgets::InputCollectionImpl(const PropertyTarget& _target, const char* _label, Container& _values, DrawElement&& _drawElement, const CollectionSettings& _settings)
{
	static_assert(!std::ranges::view<Container>, "InputCollection requires an owning container for Undo/Redo");
	static_assert(std::is_invocable_r_v<bool, DrawElement&, typename Container::value_type&, std::size_t>,
		"Collection drawing must return bool and accept (element&, index)");
	return EditProperty(_target, _values,
		&CommandHistoryWidgets::DrawCollection<Container, std::remove_reference_t<DrawElement>>,
		_label, _drawElement, _settings);
}

template<class T>
bool CommandHistoryWidgets::ResetButtonImpl(const PropertyTarget& _target, T& _value, const ResetSettings<T>& _settings)
{
	if (!_settings.getValue)
	{
		return false;
	}

	PushPropertyID(_target);
	const bool reset{ ImGui::SmallButton("Reset") };
	ImGui::PopID();
	return reset && SetPropertyValue(_target, _value, _settings.getValue());
}

// --- 内部処理 ---

template<class T, class Draw, class... Args>
bool CommandHistoryWidgets::EditProperty(const PropertyTarget& _target, T& _value, Draw _draw, Args&&... _args)
{
	PushPropertyID(_target);

	// 開始フレームの変更前の値も残せるよう、描画前にコピー
	T beforeDraw{ _value };

	// グループ全体のActive/Deactivatedを使い、ベクトルの途中の成分も拾う。
	ImGui::BeginGroup();
	bool changed{ std::invoke(_draw, *this, _value, std::forward<Args>(_args)...) };
	ImGui::EndGroup();

	if (history)
	{
		const bool active{ ImGui::IsItemActive() };

		// キーボード操作など、そのフレームだけで変更が完了する入力も記録する。
		if (active || changed)
		{
			history->BeginEdit(_target, beforeDraw);
		}

		if (!active && (ImGui::IsItemDeactivated() || changed))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
			{
				history->CancelEdit(_target);
				changed = !(_value == beforeDraw);
			}
			else
			{
				history->CommitEdit(_target, _value);
			}
		}
	}

	ImGui::PopID();
	return changed;
}

template<class T>
bool CommandHistoryWidgets::DrawNumber(T& _value, const char* _label, const NumberSettings<T>& _settings)
{
	static_assert((std::is_integral_v<T> && !std::is_same_v<T, bool>)
		|| std::is_same_v<T, float> || std::is_same_v<T, double>, "Unsupported numeric type");

	constexpr ImGuiDataType dataType{ GetNumberDataType<T>() };

	T before{ _value };
	T minimum{ _settings.min.value_or((std::numeric_limits<T>::lowest)()) };
	T maximum{ _settings.max.value_or((std::numeric_limits<T>::max)()) };
	bool changed{ false };

	if (_settings.mode == NumberSettings<T>::Mode::Input)
	{
		T step{ _settings.step.value_or(std::is_integral_v<T> ? T{ 1 } : T{}) };
		T fastStep{ _settings.fastStep.value_or(std::is_integral_v<T> ? T{ 100 } : T{}) };
		changed = ImGui::InputScalar(_label, dataType, &_value,
			step != T{} ? &step : nullptr, fastStep != T{} ? &fastStep : nullptr, _settings.format);
	}
	else if (CanUseNumberSlider(_settings))
	{
		changed = ImGui::SliderScalar(_label, dataType, &_value, &minimum, &maximum, _settings.format, MakeNumberFlags(_settings));
	}
	else
	{
		// 片側だけの制限にも対応する。スライダーの範囲が未指定ならDragを使う。
		const bool bounded{ _settings.min.has_value() || _settings.max.has_value() };
		changed = ImGui::DragScalar(_label, dataType, &_value, _settings.speed,
			bounded ? &minimum : nullptr, bounded ? &maximum : nullptr, _settings.format, MakeNumberFlags(_settings));
	}

	return changed && ValidateNumber(_value, before, _settings);
}

template<class T>
bool CommandHistoryWidgets::DrawEnum(T& _value, const char* _label, std::span<const MDGuiOption::EnumItem<T>> _items, bool _flags)
{
	const std::string preview{ MakeEnumPreview(_value, _items, _flags) };
	bool changed{ false };
	ImGui::BeginDisabled(_items.empty());
	if (ImGui::BeginCombo(_label, preview.c_str()))
	{
		for (std::size_t index = 0; index < _items.size(); ++index)
		{
			const auto& item{ _items[index] };
			const bool selected{ IsEnumItemSelected(_value, item.value, _flags) };
			const ImGuiSelectableFlags flags{ _flags ? ImGuiSelectableFlags_NoAutoClosePopups : ImGuiSelectableFlags_None };
			ImGui::PushID(static_cast<int>(index));
			if (ImGui::Selectable(item.label ? item.label : "<Unnamed>", selected, flags))
			{
				changed |= ApplyEnumSelection(_value, item.value, _flags);
			}
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}
	ImGui::EndDisabled();
	return changed;
}

template<class T>
std::string CommandHistoryWidgets::MakeEnumPreview(const T& _value, std::span<const MDGuiOption::EnumItem<T>> _items, bool _flags)
{
	if (_items.empty())
	{
		return "<No options>";
	}
	std::string preview;
	for (const auto& item : _items)
	{
		if (IsEnumItemSelected(_value, item.value, _flags))
		{
			if (!preview.empty())
			{
				preview += ", ";
			}
			preview += item.label ? item.label : "<Unnamed>";
			if (!_flags)
			{
				break;
			}
		}
	}
	if (!preview.empty())
	{
		return preview;
	}
	if constexpr (std::is_enum_v<T> || (std::is_integral_v<T> && !std::is_same_v<T, bool>))
	{
		if (_flags && GetEnumBits(_value) == 0)
		{
			return "<None>";
		}
	}
	return "<Unknown>";
}

template<class T>
bool CommandHistoryWidgets::IsEnumItemSelected(const T& _value, const T& _item, bool _flags)
{
	if constexpr (std::is_enum_v<T> || (std::is_integral_v<T> && !std::is_same_v<T, bool>))
	{
		if (_flags)
		{
			const auto value{ GetEnumBits(_value) };
			const auto mask{ GetEnumBits(_item) };
			return mask == 0 ? value == 0 : (value & mask) == mask;
		}
	}
	return _value == _item;
}

template<class T>
bool CommandHistoryWidgets::ApplyEnumSelection(T& _value, const T& _item, bool _flags)
{
	T selected{ _item };
	if constexpr (std::is_enum_v<T> || (std::is_integral_v<T> && !std::is_same_v<T, bool>))
	{
		if (_flags)
		{
			const auto value{ GetEnumBits(_value) };
			const auto mask{ GetEnumBits(_item) };
			selected = static_cast<T>(mask == 0 ? 0 : ((value & mask) == mask ? value & ~mask : value | mask));
		}
	}
	if (_value == selected)
	{
		return false;
	}
	_value = selected;
	return true;
}

template<class T>
auto CommandHistoryWidgets::GetEnumBits(T _value)
{
	if constexpr (std::is_enum_v<T>)
	{
		return static_cast<std::make_unsigned_t<std::underlying_type_t<T>>>(_value);
	}
	else
	{
		return static_cast<std::make_unsigned_t<T>>(_value);
	}
}

template<class Container, class DrawElement>
bool CommandHistoryWidgets::DrawCollection(Container& _values, const char* _label, DrawElement& _drawElement, const CollectionSettings& _settings)
{
	ImGui::PushID(_label);
	ImGui::BeginGroup();
	const float width{ ImGui::CalcItemWidth() };

	ImGui::BeginDisabled(!CanAddCollectionElement(_values, _settings));
	const bool add{ ImGui::SmallButton("+") };
	ImGui::EndDisabled();
	ImGui::SameLine();
	const std::string_view label{ _label };
	const std::string_view visibleLabel{ label.substr(0, label.find("##")) };
	const std::string_view title{ visibleLabel.empty() ? std::string_view{ "Elements" } : visibleLabel };
	ImGui::Text("%.*s (%zu)", static_cast<int>(title.size()), title.data(), _values.size());

	const std::size_t count{ _values.size() };
	std::size_t removeIndex{ count };
	std::size_t moveFrom{ count };
	std::size_t moveTo{ count };
	std::size_t index{ 0 };
	bool changed{ false };
	for (auto&& element : _values)
	{
		ImGui::PushID(static_cast<int>(index));
		ImGui::BeginGroup();
		const float rowStart{ ImGui::GetCursorScreenPos().x };
		ImGui::BeginDisabled(!CanRemoveCollectionElement(_values, _settings));
		if (ImGui::SmallButton("-"))
		{
			removeIndex = index;
		}
		ImGui::EndDisabled();
		if (_settings.reorderable)
		{
			ImGui::SameLine();
			ImGui::BeginDisabled(index == 0);
			if (ImGui::SmallButton("Up"))
			{
				moveFrom = index;
				moveTo = index - 1;
			}
			ImGui::EndDisabled();
			ImGui::SameLine();
			ImGui::BeginDisabled(index + 1 >= count);
			if (ImGui::SmallButton("Down"))
			{
				moveFrom = index;
				moveTo = index + 1;
			}
			ImGui::EndDisabled();
		}
		ImGui::SameLine();
		ImGui::Text("[%zu]", index);
		ImGui::SameLine();
		const float usedWidth{ ImGui::GetCursorScreenPos().x - rowStart };
		ImGui::SetNextItemWidth((std::max)(1.0f, width - usedWidth));
		changed |= DrawCollectionElement<typename Container::value_type>(element, index, _drawElement);
		ImGui::EndGroup();
		ImGui::PopID();
		++index;
	}

	// 入力中の参照やイテレータを壊さないよう、構造の変更は走査後に行う。
	if (add)
	{
		changed |= AddCollectionElement(_values, _settings);
	}
	else if (removeIndex < count)
	{
		changed |= RemoveCollectionElement(_values, removeIndex, _settings);
	}
	else if (moveFrom < count)
	{
		changed |= MoveCollectionElement(_values, moveFrom, moveTo, _settings);
	}
	ImGui::EndGroup();
	ImGui::PopID();
	return changed;
}

template<class Value, class Reference, class DrawElement>
bool CommandHistoryWidgets::DrawCollectionElement(Reference&& _element, std::size_t _index, DrawElement& _drawElement)
{
	if constexpr (std::is_same_v<std::remove_cvref_t<Reference>, Value>)
	{
		return std::invoke(_drawElement, _element, _index);
	}
	else
	{
		// vector<bool>のような、通常の参照ではない要素にも対応する。
		Value edited{ _element };
		const bool changed{ std::invoke(_drawElement, edited, _index) };
		if (changed)
		{
			_element = edited;
		}
		return changed;
	}
}

template<class T>
bool CommandHistoryWidgets::SetPropertyValue(const PropertyTarget& _target, T& _value, const T& _newValue)
{
	if (_value == _newValue)
	{
		return false;
	}
	if (history)
	{
		history->BeginEdit(_target, _value);
	}
	_value = _newValue;
	if (history)
	{
		history->CommitEdit(_target, _value);
	}
	return true;
}

template<class T>
int CommandHistoryWidgets::MakeNumberFlags(const NumberSettings<T>& _settings)
{
	ImGuiSliderFlags flags{ ImGuiSliderFlags_None };
	if (_settings.logarithmic)
	{
		flags |= ImGuiSliderFlags_Logarithmic;
	}
	if (_settings.clamp)
	{
		flags |= ImGuiSliderFlags_AlwaysClamp;
	}
	if (!_settings.directInput)
	{
		flags |= ImGuiSliderFlags_NoInput;
	}
	return flags;
}

template<class T>
constexpr int CommandHistoryWidgets::GetNumberDataType()
{
	if constexpr (std::is_same_v<T, float>) { return ImGuiDataType_Float; }
	else if constexpr (std::is_same_v<T, double>) { return ImGuiDataType_Double; }
	else if constexpr (sizeof(T) == 1) { return std::is_signed_v<T> ? ImGuiDataType_S8 : ImGuiDataType_U8; }
	else if constexpr (sizeof(T) == 2) { return std::is_signed_v<T> ? ImGuiDataType_S16 : ImGuiDataType_U16; }
	else if constexpr (sizeof(T) == 4) { return std::is_signed_v<T> ? ImGuiDataType_S32 : ImGuiDataType_U32; }
	else { return std::is_signed_v<T> ? ImGuiDataType_S64 : ImGuiDataType_U64; }
}

template<class T>
bool CommandHistoryWidgets::CanUseNumberSlider(const NumberSettings<T>& _settings)
{
	if (!_settings.min || !_settings.max || !(*_settings.min < *_settings.max))
	{
		return false;
	}

	const T minimum{ *_settings.min };
	const T maximum{ *_settings.max };
	if constexpr (std::is_integral_v<T> && sizeof(T) >= 4)
	{
		return minimum >= (std::numeric_limits<T>::lowest)() / 2
			&& maximum <= (std::numeric_limits<T>::max)() / 2;
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		return std::isfinite(minimum) && std::isfinite(maximum)
			&& minimum >= -(std::numeric_limits<T>::max)() / 2
			&& maximum <= (std::numeric_limits<T>::max)() / 2;
	}
	else
	{
		return true;
	}
}

template<class T>
bool CommandHistoryWidgets::ValidateNumber(T& _value, const T& _before, const NumberSettings<T>& _settings)
{
	if constexpr (std::is_floating_point_v<T>)
	{
		if (!std::isfinite(_value))
		{
			_value = _before;
			return false;
		}
	}
	if (_settings.min && _settings.max && *_settings.min > *_settings.max)
	{
		_value = _before;
		return false;
	}
	if (_settings.clamp)
	{
		if (_settings.min && _value < *_settings.min)
		{
			_value = *_settings.min;
		}
		if (_settings.max && _value > *_settings.max)
		{
			_value = *_settings.max;
		}
	}
	return _value != _before;
}

template<class Container>
bool CommandHistoryWidgets::CanAddCollectionElement(const Container& _values, const CollectionSettings& _settings)
{
	if constexpr (std::is_default_constructible_v<typename Container::value_type>
		&& requires(Container& _container) { _container.emplace_back(); })
	{
		return !_settings.fixedSize && (!_settings.maxCount || _values.size() < *_settings.maxCount)
			&& (!_settings.minCount || !_settings.maxCount || *_settings.minCount <= *_settings.maxCount);
	}
	else
	{
		return false;
	}
}

template<class Container>
bool CommandHistoryWidgets::CanRemoveCollectionElement(const Container& _values, const CollectionSettings& _settings)
{
	if constexpr (requires(Container& _container) { _container.erase(_container.begin()); })
	{
		return !_settings.fixedSize && !_values.empty()
			&& (!_settings.minCount || _values.size() > *_settings.minCount)
			&& (!_settings.minCount || !_settings.maxCount || *_settings.minCount <= *_settings.maxCount);
	}
	else
	{
		return false;
	}
}

template<class Container>
bool CommandHistoryWidgets::AddCollectionElement(Container& _values, const CollectionSettings& _settings)
{
	if constexpr (std::is_default_constructible_v<typename Container::value_type>
		&& requires(Container& _container) { _container.emplace_back(); })
	{
		if (CanAddCollectionElement(_values, _settings))
		{
			_values.emplace_back();
			return true;
		}
	}
	return false;
}

template<class Container>
bool CommandHistoryWidgets::RemoveCollectionElement(Container& _values, std::size_t _index, const CollectionSettings& _settings)
{
	if constexpr (requires(Container& _container) { _container.erase(_container.begin()); })
	{
		if (_index < _values.size() && CanRemoveCollectionElement(_values, _settings))
		{
			_values.erase(std::next(_values.begin(), static_cast<std::ptrdiff_t>(_index)));
			return true;
		}
	}
	return false;
}

template<class Container>
bool CommandHistoryWidgets::MoveCollectionElement(Container& _values, std::size_t _from, std::size_t _to, const CollectionSettings& _settings)
{
	if constexpr (std::is_swappable_v<typename Container::value_type>)
	{
		if (!_settings.reorderable || _from >= _values.size() || _to >= _values.size() || _from == _to)
		{
			return false;
		}
		auto from{ std::next(_values.begin(), static_cast<std::ptrdiff_t>(_from)) };
		auto to{ std::next(_values.begin(), static_cast<std::ptrdiff_t>(_to)) };
		if (_from < _to)
		{
			std::rotate(from, std::next(from), std::next(to));
		}
		else
		{
			std::rotate(to, from, std::next(from));
		}
		return true;
	}
	else
	{
		return false;
	}
}

template<std::size_t Count>
bool CommandHistoryWidgets::DrawVectorComponents(const char* _label, const std::array<float*, Count>& _components, const FloatSettings& _numberSettings, const VectorSettings& _settings)
{
	ImGui::PushID(_label);
	ImGui::BeginGroup();

	const float spacing{ ImGui::GetStyle().ItemInnerSpacing.x };
	float labelWidth{ 0.0f };
	for (std::size_t index = 0; index < Count; ++index)
	{
		const char* axis{ _settings.axisLabels[index] };
		if (axis && *axis)
		{
			labelWidth += ImGui::CalcTextSize(axis).x + spacing;
		}
	}
	const float width{ (std::max)(1.0f,
		(ImGui::CalcItemWidth() - labelWidth - spacing * static_cast<float>(Count - 1)) / static_cast<float>(Count)) };

	bool changed{ false };
	for (std::size_t index = 0; index < Count; ++index)
	{
		ImGui::PushID(static_cast<int>(index));
		if (index != 0)
		{
			ImGui::SameLine(0.0f, spacing);
		}
		const char* axis{ _settings.axisLabels[index] };
		if (axis && *axis)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(axis);
			ImGui::SameLine(0.0f, spacing);
		}
		ImGui::SetNextItemWidth(width);
		changed |= DrawNumber(*_components[index], "##Value", _numberSettings);
		ImGui::PopID();
	}

	const std::string_view label{ _label };
	const std::string_view visibleLabel{ label.substr(0, label.find("##")) };
	if (!visibleLabel.empty())
	{
		ImGui::SameLine(0.0f, spacing);
		ImGui::TextUnformatted(visibleLabel.data(), visibleLabel.data() + visibleLabel.size());
	}
	ImGui::EndGroup();
	ImGui::PopID();
	return changed;
}