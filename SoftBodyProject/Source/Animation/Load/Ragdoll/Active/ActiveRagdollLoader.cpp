#include <filesystem>
#include <memory>
#include <sstream>
#include <string_view>
#include <utility>
#include <windows.h>

#include <document.h>
#include <error/en.h>
#include <filereadstream.h>

#include "RagdollLoader.h"

#include "ActiveRagdollLoader.h"
#include "ActiveRagdollLoaderHelper.h"

bool ActiveRagdollLoader::Load(const std::string& _path, ActiveRagdollDefinition& _output)
{
	// ActiveRagdoll定義JSONを開く
	FILE* file{ nullptr };
	fopen_s(&file, _path.c_str(), "rb");
	if (file == nullptr)
	{
		return false;
	}

	const size_t BUFFER_SIZE{ 65536 };
	std::unique_ptr<char[]> buffer{ std::make_unique<char[]>(BUFFER_SIZE) };
	rapidjson::FileReadStream stream{ file, buffer.get(), BUFFER_SIZE };

	// JSONを解析してファイルを閉じる
	rapidjson::Document document;
	document.ParseStream(stream);
	fclose(file);

	// JSONの構文エラーをログへ出力する
	if (document.HasParseError())
	{
		const char* message{ rapidjson::GetParseError_En(document.GetParseError()) };
		size_t offset{ document.GetErrorOffset() };

		std::ostringstream oss;
		oss << "JSON parse error: " << message << " offset: " << offset;
		OutputDebugStringA(oss.str().c_str());
		OutputDebugStringA("\n");
		return false;
	}

	// RootとFormatがActiveRagdoll定義として正しいか確認する
	if (!document.IsObject() || !document.HasMember("Format") || !document["Format"].IsString())
	{
		return false;
	}

	std::string_view format{ document["Format"].GetString(), document["Format"].GetStringLength() };
	if (format != "ActiveRagdollDefinition")
	{
		return false;
	}

	// 出力を書き換える前に必要な定義をすべて読み込む
	ActiveRagdollDefinition loaded;
	std::string ragdollPathString;
	if (!ActiveRagdollLoaderHelper::LoadDefinitionData(document, ragdollPathString, loaded.settings))
	{
		return false;
	}

	// Ragdoll定義の相対パスはActiveRagdoll定義JSONの場所を基準に解決する
	std::filesystem::path ragdollPath{ ragdollPathString };
	if (ragdollPath.is_relative())
	{
		ragdollPath = std::filesystem::path{ _path }.parent_path() / ragdollPath;
	}
	ragdollPath = ragdollPath.lexically_normal();

	// 既存のRagdollLoaderでRagdoll定義を読み込む
	if (!RagdollLoader::Load(ragdollPath.string(), loaded.ragdollDefinition))
	{
		return false;
	}

	_output = std::move(loaded);
	return true;
}