#pragma once

#include <utility>

#include "FieldMacros.h"

#include "MDJson.h"

template<class T>
MDJson::Value MDJson::ToJson(const std::vector<T>& _value, Allocator& _allocator)
{
	rapidjson::Value result{ CreateArray()};

	for (auto& value : _value)
	{
		Append(result, ToJson(value, _allocator), _allocator);
	}

	return result;
}

template<class T>
	requires requires { T::GetDocumentInfo(); }
MDJson::Value MDJson::ToJson(const T& _value, Allocator& _allocator)
{
	return SaveDocument(_value, _allocator);
}

template<class Document>
MDJson::Value MDJson::SaveDocument(const Document& _document, Allocator& _allocator)
{
	rapidjson::Value result{ CreateObject()};

	MDReflection::ForEachField(
		_document,
		[&](const auto& field, const auto& value)
		{
			rapidjson::Value key;
			key.SetString(field.name, _allocator);

			rapidjson::Value jsonValue{ ToJson(value, _allocator) };

			result.AddMember(key, jsonValue, _allocator);
		}
	);

	return result;
}

template<class T>
bool MDJson::FromJson(const Value& _json, std::vector<T>& _value)
{
	if (!_json.IsArray())
	{
		return false;
	}

	std::vector<T> result;
	result.reserve(_json.Size());

	for (const auto& jsonValue : _json.GetArray())
	{
		T value{};

		if (!FromJson(jsonValue, value))
		{
			return false;
		}

		result.push_back(std::move(value));
	}

	_value = std::move(result);
	return true;
}

template<class T>
	requires requires { T::GetDocumentInfo(); }
bool MDJson::FromJson(const Value& _json, T& _value)
{
	return LoadDocument(_json, _value);
}

template<class Document>
bool MDJson::LoadDocument(const Value& _json, Document& _document)
{
	if (!_json.IsObject())
	{
		return false;
	}

	Document temporary{ _document };
	bool succeeded{ true };

	MDReflection::ForEachField(
		temporary,
		[&](const auto& field, auto& value)
		{
			if (!succeeded)
			{
				return;
			}

			const auto member = _json.FindMember(field.name);

			if (member == _json.MemberEnd() ||
				!FromJson(member->value, value))
			{
				succeeded = false;
			}
		}
	);

	if (!succeeded)
	{
		return false;
	}

	_document = std::move(temporary);
	return true;
}

template<class T>
bool MDJson::FromJson(const Value& _json, const char* _name, T& _output)
{
	const Value* value{ FindValue(_json, _name) };

	return value && FromJson(*value, _output);
}

template<class T>
bool MDJson::AddMember(Value& _json, const char* _name, const T& _value, Allocator& _allocator)
{
	Value jsonValue{ ToJson(_value, _allocator) };
	return AddMember(_json, _name, std::move(jsonValue), _allocator);
}

template<class T>
bool MDJson::Append(Value& _json, const T& _value, Allocator& _allocator)
{
	Value jsonValue{ ToJson(_value, _allocator) };
	return Append(_json, std::move(jsonValue), _allocator);
}