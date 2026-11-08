#pragma once

#include <filesystem>
#include <span>
#include <unordered_set>
#include <vector>

#include "Object/ObjectDocument.h"

#include "MDJson.h"

struct SceneDocument;

class SceneSerializer
{
public:
	/*
		保存(作成したいファイルのパスを渡す)
		　- ファイル名まで渡す
		  - 対応するファイルがあるならそれを上書きする
		  - 対応するファイルがないなら新しく作る
		  - 戻り値は成功判定
	*/ 
	static bool Save(const SceneDocument& _scene, std::filesystem::path _filePath);

	/*
		ロード(ロードしたいファイルのパスを渡す)
		  - 戻り値は成功判定
	*/
	static bool Load(SceneDocument& _scene, std::filesystem::path _filePath);


	/*
		オブジェクトのJson作成処理
		filePathを正しく渡すことでJsonを作ることも出来る
	*/
	static bool SaveObject(const ObjectDocument& _object, MDJson::Value& _output, MDJson::Allocator& _allocator, std::filesystem::path _filePath = {});
	// オブジェクトのロード関数
	static bool LoadObject(ObjectDocument& _object, const  MDJson::Value& _output);
	// オブジェクトのロード関数(Json版)
	static bool LoadObject(ObjectDocument& _object, std::filesystem::path _filePath = {});
	// 親Objectと、その子孫をChildrenに含めて保存する。SceneDocumentは不要。
	static bool SaveObject(const ObjectDocument& _object, std::span<const ObjectDocument> _objects, MDJson::Value& _output, MDJson::Allocator& _allocator, std::filesystem::path _filePath = {});
	// 親を先頭に、子孫を含むObject列として復元する。成功時だけ出力を置き換える。
	static bool LoadObject(std::vector<ObjectDocument>& _objects, const MDJson::Value& _input);
	static bool LoadObject(std::vector<ObjectDocument>& _objects, std::filesystem::path _filePath);
private:
	static bool SaveObjectHierarchy(const ObjectDocument& _object, std::span<const ObjectDocument> _objects, MDJson::Value& _output, MDJson::Allocator& _allocator, std::unordered_set<std::uint64_t>& _savedIDs);
	static bool LoadObjectHierarchy(std::vector<ObjectDocument>& _objects, const MDJson::Value& _input, std::uint64_t _parentID, std::unordered_set<std::uint64_t>& _loadedIDs);
};