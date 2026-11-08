#pragma once

#include <string>
#include <tuple>

#include "LoadSettingFunc.h"
#include "InspectorFieldDrawFunc.h"

#include "CommandHistoryWidgets.h"

#include "ComponentDocument.h"

template<class Derived>
std::unique_ptr<ComponentDocumentBase> ComponentDocument<Derived>::Clone(std::uint64_t _ownerObjectID)
{
	const Derived& source{ static_cast<const Derived&>(*this) };

	auto result{ std::make_unique<Derived>(_ownerObjectID, id) };

	// 保存対象のFieldだけ複製し、実行時リソースはOnChangeで取得する。
	MDReflection::ForEachField(
		*result,
		[&source](const auto& _field, auto& _value)
		{
			_value = source.*_field.member;
		}
	);
	result->OnChange();

	return result;
}

template<class Derived>
MDJson::Value ComponentDocument<Derived>::SaveJson(MDJson::Allocator& _allocator) const
{
	const auto& document = static_cast<const Derived&>(*this);

	// 共通処理で、Documentに定義した各フィールドを保存する
	auto result{ MDJson::SaveDocument(document, _allocator) };

	// Componentの名前は、Documentの定義から取得する
	static constexpr auto info{ Derived::GetDocumentInfo() };

	if (!MDJson::AddMember(result, "Name", info.name, _allocator))
	{
		return MDJson::Value{};
	}

	return result;
}

template<class Derived>
bool ComponentDocument<Derived>::LoadJson(const MDJson::Value& _json)
{
	Derived& document{ static_cast<Derived&>(*this) };

	if (!MDJson::LoadDocument(_json, document))
	{
		return false;
	}

	OnChange();
	return true;
}

template<class Derived>
bool ComponentDocument<Derived>::DrawInspector(const SceneDocument& _scene, bool& _removeRequested, bool _removable)
{
	PropertyTarget target;
	target.objectID = ownerObjectID;
	target.componentID = id;

	GroupSettings settings;
	settings.selectable = true;
	settings.closable = _removable;

	static constexpr auto info{ Derived::GetDocumentInfo() };
	const auto header = CommandHistoryWidgets::Header(target, info.name, settings);
	_removeRequested = header.closed;
	if (!header.open || header.closed)
	{
		return header.clicked;
	}

	std::string_view groupName;
	bool groupOpen{ true };
	bool tableOpen{ false };

	bool changed{ false };
	DrawInspectorFields<0>(_scene, target, groupName, groupOpen, tableOpen, changed);

	if (tableOpen)
	{
		CommandHistoryWidgets::EndPropertyTable();
	}

	if (changed)
	{
		OnChange();
	}

	return header.clicked;
}


template<class Derived>
void* ComponentDocument<Derived>::FindFieldAddress(std::string_view _fieldName)
{
	Derived& document{ static_cast<Derived&>(*this) };
	void* result{ nullptr };

	MDReflection::ForEachField(
		document,
		[&](const auto& _field, auto& _value)
		{
			if (_fieldName == _field.name)
			{
				result = std::addressof(_value);
				return;
			}
		}
	);

	return result;
}

template<class Derived>
template<std::size_t Index>
void ComponentDocument<Derived>::DrawInspectorFields(const SceneDocument& _scene, const PropertyTarget& _target, std::string_view& _groupName, bool& _groupOpen, bool& _tableOpen, bool& _changed)
{
	static constexpr auto info{ Derived::GetDocumentInfo() };
	if constexpr (Index < std::tuple_size_v<decltype(info.fields)>)
	{
		Derived& document{ static_cast<Derived&>(*this) };
		const auto& field{ std::get<Index>(info.fields) };
		const auto row{ LoadSettingFunc::Load<RowSettings>(document, field.guiOptions) };

		if (row.visible)
		{
			const auto group{ LoadSettingFunc::Load<GroupSettings>(field.guiOptions) };
			const std::string_view groupName{ group.name ? group.name : "" };

			PropertyTarget fieldTarget{ _target };
			fieldTarget.fieldName = field.name;

			if (groupName != _groupName)
			{
				if (_tableOpen)
				{
					CommandHistoryWidgets::EndPropertyTable();
					_tableOpen = false;
				}

				_groupName = groupName;
				_groupOpen = true;

				if (!groupName.empty())
				{
					PropertyTarget groupTarget{ _target };
					groupTarget.fieldName = "Group/" + std::string{ field.name };

					GroupSettings groupHeader{ group };
					groupHeader.header = nullptr;
					_groupOpen = CommandHistoryWidgets::Header(groupTarget, group.name, groupHeader);
				}
			}

			if (_groupOpen)
			{
				if (group.header && *group.header)
				{
					if (_tableOpen)
					{
						CommandHistoryWidgets::EndPropertyTable();
						_tableOpen = false;
					}

					GroupSettings heading;
					heading.foldout = false;
					CommandHistoryWidgets::Header(fieldTarget, group.header, heading);
				}

				if (!_tableOpen)
				{
					// このテーブルの先頭のFieldから、テーブル全体の設定を取得する。
					const auto table{ LoadSettingFunc::Load<TableSettings>(field.guiOptions) };
					_tableOpen = CommandHistoryWidgets::BeginPropertyTable(fieldTarget, table);
				}

				if (_tableOpen)
				{
					if (InspectorFieldDrawFunc::DrawField(document, _scene, fieldTarget, field))
					{
						_changed = true;
					}
				}
			}
		}

		DrawInspectorFields<Index + 1>(_scene, _target, _groupName, _groupOpen, _tableOpen, _changed);
	}
}