#pragma once

#include "PanelBase.h"
#include "EditorCamera.h"

#include "ModelHandle.h"

class ModelPrefabToolPanel :public PanelBase
{
public:
	ModelPrefabToolPanel(EditorDocumentService& _documentService);

private:
	void DrawInternal() override;
	// フォーカスされたタイミングで呼ばれる関数
	void OnForcused() override;
	// パネルが閉じられたタイミングで呼ばれる関数。
	void OnClosed() override;
	// モデル選択ボタン
	void DrawSelectedModelButton();
	// Model / Frameの表示方式切り替え
	void DrawModelModeButton();
	// Panel内右下の保存・読み込みボタン
	void DrawFileButtons();
	// 編集中の親Objectと子孫を、ひとつのObjectアセットとして保存する。
	void SaveObject();
	// 読み込み成功後だけ、編集履歴とモデル選択を更新する。
	void LoadObject();
	// 読み込んだRendererに合わせ、モデル名と切り替えボタンの状態を復元する。
	void UpdateModelSelection();
	// ModelRendererの場合のドキュメント作成関数
	void CreateModelDocument();
	// FrameRendererの場合のドキュメント作成関数
	void CreateFrameDocument();
	
private:
	GizmoContext context;
	EditorCamera camera;

	std::uint64_t rootObjectID{ 0 };
	std::string currentModelName;
	ModelHandle currentModel;
	bool isFrameMode{ false };
	std::string fileOperationMessage;
};