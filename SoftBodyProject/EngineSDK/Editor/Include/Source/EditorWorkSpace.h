#pragma once

#include <filesystem>

#include "EditorDocumentService.h"

#include "Panel/HierarchyPanel.h"
#include "Panel/InspectorPanel.h"
#include "Panel/ScenePanel.h"
#include "Panel/AssetPanel.h"
#include "Panel/Tool/ModelLoader/ModelPrefabToolPanel.h"

class EditorWorkSpace
{
public:
	EditorWorkSpace();
	bool InitializeScenePreview();
	void ShutdownScenePreview();
	void Draw();
private:
	void LoadAllModel();
	void DrawMenu();

	void NewScene();

	void OpenScene();
	void OpenScene(std::filesystem::path _path);

	void SaveScene();
	void SaveScene(std::filesystem::path _path);

	void SaveSceneAs();
private:
	EditorDocumentService documentService;

	HierarchyPanel hierarchy;
	InspectorPanel inspector;
	ScenePanel scene;
	AssetPanel asset;

	ModelPrefabToolPanel modelTool;

	std::filesystem::path currentScenePath;
};