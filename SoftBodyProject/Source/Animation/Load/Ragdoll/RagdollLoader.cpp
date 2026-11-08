#include <sstream>
#include <windows.h>

#include <document.h>
#include <filereadstream.h>
#include <error/en.h>

#include "RagdollLoader.h"

#include "RagdollLoaderHelper.h"

bool RagdollLoader::Load(const std::string& _path, RagdollDefinition& _output)
{
	// ファイル
	FILE* file{ nullptr };
	fopen_s(&file, _path.c_str(), "rb");

	if (file == nullptr)
	{
		return false;
	}

	const size_t BUFFER_SIZE{ 65536 };

	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(BUFFER_SIZE);
	// 変換準備
	rapidjson::FileReadStream stream{ file, buffer.get(), BUFFER_SIZE };

	// 変換
	rapidjson::Document document;
	document.ParseStream(stream);
	// ファイルを閉じる
	fclose(file);

	// ちゃんと変換できたかチェック
	if (document.HasParseError())
	{
		// エラーの場合はそれをログに出す
		const char* message = rapidjson::GetParseError_En(document.GetParseError());
		size_t offset = document.GetErrorOffset();

		std::ostringstream oss;
		oss << "JSON parse error: " << message
			<< " offset: " << offset;

		OutputDebugStringA(oss.str().c_str());
		OutputDebugStringA("\n");

		return false;
	}

	// ちゃんとオブジェクトがあるか確認
	if (!document.IsObject())
	{
		return false;
	}

	// フォーマットの確認
	if (!document.HasMember("Format") || !document["Format"].IsString())
	{
		return false;
	}

	std::string_view format{ document["Format"].GetString(),document["Format"].GetStringLength() };

	if (format != "RagdollDefinition")
	{
		return false;
	}

	RagdollDefinition loaded;

	if (!RagdollLoaderHelper::LoadRagdollDefinition(document, loaded))
	{
		return false;
	}

	_output = std::move(loaded);
	return true;
}