#pragma once

#include <vector>
#include <memory>

#include "DocumentSession.h"
#include "CollisionSettingDocument.h"

class EditorDocumentService
{
public:
	EditorDocumentService();

	SceneDocument& GetCurrentDocument();
	CommandHistory& GetCurrentCommandHistory();

	SceneDocument& GetSceneDocument();
	CommandHistory& GetSceneCommandHistory();

	CollisionSettingDocument& GetCollisionSetting();

	// ドキュメントを新しく作成
	std::uint64_t CreateDocument();
	// 現在のドキュメントを変更
	void SetCurrentDocument(std::uint64_t _id);
	// 対応するIDのドキュメントを破棄
	void ReleaseDocument(std::uint64_t _id);
	void Clear();
private:
	std::uint64_t currentSceneIndex{ 0 };
	std::vector<std::unique_ptr<DocumentSession>> sessions;
	std::vector<std::uint64_t> freeIndices;
	CollisionSettingDocument collisionSetting;
};