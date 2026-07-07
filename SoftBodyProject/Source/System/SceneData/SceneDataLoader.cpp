#include <cstdio>
#include <sstream>
#include <windows.h>

#include <document.h>
#include <filereadstream.h>
#include <error/en.h>

#include "JsonLoadHelperFunc.h"

#include "SceneDataLoader.h"

bool SceneDataLoader::LoadJson(std::string _filePath, SceneFileData& _output)
{
	// ファイル
	FILE* file{ nullptr };
	fopen_s(&file, _filePath.c_str(), "rb");

	if (file == nullptr)
	{
		return false;
	}

	const size_t BUFFER_SIZE{ 65536 };

	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(BUFFER_SIZE);
	// 変換準備
	rapidjson::FileReadStream stream{ file, buffer.get(), BUFFER_SIZE};

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

	// あるならデータを埋めていく
	// バージョン
	// 項目があるか、それが数値かチェック
	if (!document.HasMember("Version") || !document["Version"].IsNumber())
	{
		return false;
	}

	_output.version = document["Version"].GetInt();

	// オブジェクトを追加していく。
	for (auto& obj : document["Objects"].GetArray())
	{
		SceneObjectData objData;
		
		// 最低限必要な情報があるかチェック
		if (!obj.HasMember("ID") || !obj["ID"].IsString())
		{
			continue;
		}
		if (!obj.HasMember("Type") || !obj["Type"].IsString())
		{
			continue;
		}

		objData.id = obj["ID"].GetString();
		objData.type = obj["Type"].GetString();
		
		for (auto& component : document["Components"].GetArray())
		{
			// 名前のチェックは忘れんなよ。
			if (!component.HasMember("Name") || !component["Name"].IsString())
			{
				continue;
			}
			// 作成をしていく―
			objData.component.push_back(std::move(JsonLoadHelperFunc::CreateFuncs[component["Name"].GetString()](component)));
		}

		_output.objectDatas.push_back(std::move(objData));
	}
}
