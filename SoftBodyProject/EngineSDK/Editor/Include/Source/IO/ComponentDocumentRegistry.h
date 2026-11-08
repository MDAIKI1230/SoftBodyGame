#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Base/ComponentDocumentBase.h"

class ComponentDocumentRegistry
{
public:
	// 名前に対応したドキュメント作成
	static std::unique_ptr<ComponentDocumentBase> Create(const std::string& _name, std::uint64_t _ownerObjectID, std::uint64_t _id);
	// 登録済みのコンポーネント名を名前順で取得
	static std::vector<std::string> GetComponentNames();

	// すべてのコンポーネント作成関数追加
	static bool RegisterAll();

private:
	using CreateFunc = std::unique_ptr<ComponentDocumentBase>(*)(std::uint64_t, std::uint64_t);

	using CreateFuncMap = std::unordered_map<std::string, CreateFunc>;

	template<class T>
	static bool Register()
	{
		static constexpr auto info{ T::GetDocumentInfo() };

		CreateFunc create{
			[](std::uint64_t _ownerObjectID, std::uint64_t _id) -> std::unique_ptr<ComponentDocumentBase>
			{
				return std::make_unique<T>(_ownerObjectID, _id);
			}
		};

		return GetCreateFuncs()
			.try_emplace(info.name, create)
			.second;
	}

	static CreateFuncMap& GetCreateFuncs()
	{
		static CreateFuncMap funcs;
		return funcs;
	}
};