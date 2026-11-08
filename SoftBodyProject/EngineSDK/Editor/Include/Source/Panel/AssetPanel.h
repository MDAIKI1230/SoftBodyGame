#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "Base/PanelBase.h"
#include "MDJson.h"

class AssetPanel : public PanelBase
{
public:
	AssetPanel(EditorDocumentService& _documentService);

	// 直前に描いた項目をドロップ先にし、確定したObjectアセットのパスを返す。
	static bool AcceptDropPath(std::filesystem::path& _assetPath);
	// Objectと子孫を新しいIDで追加し、作成した親ObjectのIDを返す。失敗時は0。
	static std::uint64_t CreateObjectFromAsset(SceneDocument& _document, const std::filesystem::path& _assetPath, std::string& _errorMessage);

private:
	void DrawInternal() override;
	void OnForcused() override;
	void RefreshAssets();
	void DrawDirectory(
		const std::filesystem::path& _directory,
		const std::vector<std::filesystem::path>& _directories,
		const std::vector<std::filesystem::path>& _files);
	void DrawAssetFile(const std::filesystem::path& _filePath);

	static std::string PathToUtf8(const std::filesystem::path& _path);
	static bool CollectObjectIDs(const MDJson::Value& _input, std::unordered_map<std::uint64_t, std::uint64_t>& _objectIDs);
	static bool RemapObjectIDs(MDJson::Value& _input, const std::unordered_map<std::uint64_t, std::uint64_t>& _objectIDs, MDJson::Allocator& _allocator);
	static bool RemapReferences(MDJson::Value& _input, const std::unordered_map<std::uint64_t, std::uint64_t>& _objectIDs);

private:
	const std::vector<std::filesystem::path>& gameObjectFiles;

private:
	std::vector<std::filesystem::path> gameObjectDirectories;
	std::filesystem::path rootDirectory{ L"Res/Data/Objects" };
	std::filesystem::path selectedAsset;
	std::vector<std::filesystem::path> directories;
	std::vector<std::filesystem::path> assetFiles;
	std::string directoryMessage;
	std::string operationMessage;
};