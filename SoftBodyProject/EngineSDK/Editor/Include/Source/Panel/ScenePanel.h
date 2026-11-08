#pragma once

#include "Base/PanelBase.h"
#include "GizmoContext.h"
#include "EditorCamera.h"

class ScenePanel : public PanelBase
{
public:
	ScenePanel(EditorDocumentService& _documentService);
private:
	void DrawInternal() override;

	// プレビュー画像にドロップされたObjectアセットを配置する。
	void DrawAssetDropTarget(SceneDocument& _document);
	void DrawGizmoToolbar(SceneDocument& _document);
	void DrawGizmo(SceneDocument& _document);

	void OnForcused() override;
private:
	GizmoContext context;
	EditorCamera camera;
	std::string assetDropMessage;
};