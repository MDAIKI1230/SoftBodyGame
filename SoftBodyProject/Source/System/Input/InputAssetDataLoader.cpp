#include <sstream>
#include <windows.h>

#include <document.h>
#include <filereadstream.h>
#include <error/en.h>

#include "InputAssetDataLoadHelper.h"

#include "InputAssetDataLoader.h"

bool InputAssetDataLoader::LoadAsset(std::string _path, InputActionAssetData& _output)
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

	if (format != "InputActionAsset")
	{
		return false;
	}

	InputActionAssetData loaded;

	// バージョン
	if (!document.HasMember("Version") || !document["Version"].IsUint())
	{
		return false;
	}

	loaded.version = document["Version"].GetUint();

	// ID
	if (!document.HasMember("ID") || !document["ID"].IsUint64())
	{
		return false;
	}

	loaded.id = InputDataID{ document["ID"].GetUint64() };

	// 名前
	if (!document.HasMember("Name") || !document["Name"].IsString())
	{
		return false;
	}

	loaded.name = document["Name"].GetString();

	// 入力デバイスの種類
	if (!InputAssetDataLoadHelper::GetControlSchemes(document, loaded.controlSchemes))
	{
		return false;
	}

	// 読めたかチェック
	if (loaded.controlSchemes.size() == 0)
	{
		return false;
	}

	// 入力アクションマップ情報
	if (!InputAssetDataLoadHelper::GetActionMaps(document, loaded.actionMaps))
	{
		return false;
	}

	// 読めたかチェック
	if (loaded.actionMaps.size() == 0)
	{
		return false;
	}

	_output = std::move(loaded);

	return true;
}
