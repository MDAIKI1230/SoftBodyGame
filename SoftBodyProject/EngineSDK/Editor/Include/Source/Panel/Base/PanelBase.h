#pragma once

#include <d3d11.h>

#include "EditorDocumentService.h"

class PanelBase
{
public:
	// コンストラクタ
	PanelBase(EditorDocumentService& _documentService, std::string _panelName) :
		documentService{ _documentService },
		panelName{ _panelName }
	{
	}

	void Draw();

	// 状態の更新とイベント通知
	void UpdateState();
	// 見せるか設定
	bool* GetIsShowPtr() { return &isShow; }
	// パネルの描画サイズを修正する関数
	bool ResizeScenePreview(int width, int height);
	// リソースの解放
	void Release();
	// Gizmo操作終了
	void FinishGizmoEdit();

	// デストラクタ
	virtual ~PanelBase();
protected:
	// 実際の描画部分
	virtual void DrawInternal() = 0;
	// フォーカスされたタイミングで呼ばれる関数
	virtual void OnForcused() {};
	// パネルが閉じられたタイミングで呼ばれる関数。
	virtual void OnClosed() {};

protected:
	// 見せるかフラグ
	bool isShow{ true };

	// 描画先画像情報ハンドル
	int screen{ -1 };
	// 描画先を渡すための変数
	ID3D11ShaderResourceView* srv{ nullptr };

	// スクリーンの大きさ
	int sceneWidth{ 0 };
	int sceneHeight{ 0 };

	// シーン情報
	EditorDocumentService& documentService;

	// 対応シーンドキュメントID(未変更時はSceneDocumentを示す0が入ってる)
	std::uint64_t documentID{ 0 };

	std::string panelName;
private:
	bool wasForcused{ false };
	bool wasShow{ false };
};