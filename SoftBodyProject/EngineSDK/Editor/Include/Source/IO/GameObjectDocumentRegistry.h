#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "FixedString.h"

#include "ObjectDocument.h"

class GameObjectDocumentRegistry
{
public:
	// 名前に対応したドキュメント作成
	static ObjectDocument Create(const std::string& _name, std::uint64_t _id);
	// 名前に対応したドキュメント作成
	static ObjectDocument CreateDefault(const std::string& _name, std::uint64_t _id);
	// 登録済みのオブジェクト名を名前順で取得
	static std::vector<std::string> GetGameObjectNames();
	// 登録済みのオブジェクトのパスを名前順で取得
	static const std::vector<std::filesystem::path>& GetGameObjectPaths();
	// すべてのオブジェクト作成関数追加
	static bool RegisterAll();

private:
	using CreateFunc = ObjectDocument(*)(std::uint64_t, bool);

	using CreateFuncMap = std::unordered_map<std::string, CreateFunc>;

	template<class T, FixedString Name>
	static bool Register()
	{
		CreateFunc create{
			[](std::uint64_t _id, bool _withDefaults) -> ObjectDocument
			{
				ObjectDocument object{ _id ,Name.ToString() };
				object.name = Name.ToString();

				if constexpr (requires { T::GetComponentInfo(); })
				{
					if (_withDefaults)
					{
						constexpr auto components{ T::GetComponentInfo() };
						std::apply(
							[&object](const auto&... _names)
							{
								(static_cast<void>(object.AddComponent(_names)), ...);
							},
							components
						);
					}
				}
				
				return object;
			}
		};

		return GetCreateFuncs()
			.try_emplace(Name.ToString(), create)
			.second;
	}

	static CreateFuncMap& GetCreateFuncs()
	{
		static CreateFuncMap funcs;
		return funcs;
	}
};