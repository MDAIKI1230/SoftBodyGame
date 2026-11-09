#pragma once

#include <string_view>
#include <vector>
#include <document.h>
#include <filesystem>
#include <span>

#include "LightConstants.h"
#include "MDMath.h"

/*
	Jsonの読み書きをするための関数群
	変数もこの中の物を使うこと
*/
namespace MDJson
{
	using Allocator = rapidjson::Document::AllocatorType;
	using Value = rapidjson::Value;
	using JsonDocument = rapidjson::Document;
	using ArrayView = std::span<const Value>;

	JsonDocument CreateDocument();
	Value CreateObject();
	Value CreateArray();

	Allocator& GetAllocator(JsonDocument& _json);

	bool IsObject(const Value& _json);
	bool IsArray(const Value& _json);

	// Vector3 → {"X": ..., "Y": ..., "Z": ...}
	Value ToJson(const Vector3& _value, Allocator& _allocator);
	Value ToJson(float _value, Allocator& _allocator);
	Value ToJson(std::string_view _value, Allocator& _allocator);
	Value ToJson(int _value, Allocator& _allocator);
	Value ToJson(LightType _type, Allocator& _allocator);
	Value ToJson(bool _value, Allocator& _allocator);
	Value ToJson(const char* _value, Allocator& _allocator);
	Value ToJson(std::uint64_t _value, Allocator& _allocator);
	Value ToJson(std::uint8_t _value, Allocator& _allocator);
	template<class T>
	Value ToJson(const std::vector<T>& _value, Allocator& _allocator);
	template<class T>
		requires requires { T::GetDocumentInfo(); }
	Value ToJson(const T& _value, Allocator& _allocator);

	// 公開された項目をJSONオブジェクトへ変換
	template<class Document>
	Value SaveDocument(const Document& _document, Allocator& _allocator);

	bool SaveFile(const Value& _json, const std::filesystem::path& _filePath);

	bool FromJson(const Value& _json, float& _value);
	bool FromJson(const Value& _json, Vector3& _value);
	bool FromJson(const Value& _json, int& _value);
	bool FromJson(const Value& _json, LightType& _type);
	bool FromJson(const Value& _json, bool& _value);
	bool FromJson(const Value& _json, std::string& _value);
	bool FromJson(const Value& _json, std::string_view& _value);
	bool FromJson(const Value& _json, std::uint64_t& _value);
	bool FromJson(const Value& _json, std::uint8_t& _value);
	bool FromJson(const Value& _json, ArrayView& _value);
	template<class T>
	bool FromJson(const Value& _json, std::vector<T>& _value);
	template<class T>
		requires requires { T::GetDocumentInfo(); }
	bool FromJson(const Value& _json, T& _value);


	template<class Document>
	bool LoadDocument(const Value& _json, Document& _document);

	bool LoadFile(JsonDocument& _json, const std::filesystem::path& _filePath);

	const Value* FindValue(const Value& _json, const char* _name);
	template<class T>
	bool FromJson(const Value& _json, const char* _name, T& _output);

	bool AddMember(Value& _json, const char* _name, Value&& _value, Allocator& _allocator);
	template<class T>
	bool AddMember(Value& _json, const char* _name, const T& _value, Allocator& _allocator);

	bool Append(Value& _json, Value&& _value, Allocator& _allocator);
	template<class T>
	bool Append(Value& _json, const T& _value, Allocator& _allocator);
}

#include "MDJson.inl"