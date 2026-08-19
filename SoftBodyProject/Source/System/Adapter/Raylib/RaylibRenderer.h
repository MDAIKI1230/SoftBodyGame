#include <unordered_map>

#include "IRenderer.h"

#include "RaylibInclude.h"

class RaylibRenderer :public IRenderer
{
public:
	// カメラ関連
	// カメラセット
	virtual void SetCamera(const Camera* _camera) override;
	// ---ダブルバッファリング用---

	// 垂直同期信号を待つ
	virtual int ClearDrawScreen() override;
	// ダブルバッファリングの反転
	virtual int ScreenFlip() override;
	// --- 設定 ---

	// ZDepth使う
	virtual int SetUseZDepth(bool _flag) override;
	// ZDepth書き込み
	virtual int SetWriteZDepth(bool _flag) override;

	// ---読み込み関数---

	// モデルの読み込み
	virtual int LoadModel(const std::string& _fileName) override;
	// 画像の読み込み
	virtual int LoadGraph(const std::string& _fileName) override;
	/// <summary>
	/// 画像の分割読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_allNum">分割全体数</param>
	/// <param name="_xNum">横の数</param>
	/// <param name="_yNum">縦の数</param>
	/// <param name="_xSize">分割した一つの横幅</param>
	/// <param name="_ySize">分割した一つの縦幅</param>
	/// <param name="handleBuf">配列のアドレス</param>
	virtual void LoadDivGraph(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf) override;
	// モデル情報セット系
	// 行列セット
	virtual void ModelSetMatrix(int _handle, const Matrix4x4& _mat) override;
	// ---描画関数---

	// モデル描画
	virtual void DrawModel(int _handle) override;
	// 画像描画
	virtual void DrawGraph(const Vector2& _pos, int _handle, bool _transFlag) override;
	// 球描画
	virtual void DrawSphere(const Vector3& _pos, float _radius, const Color& _color) override;
	// メッシュ球描画
	void DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color) override;
	// Box描画
	virtual void DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color) override;
	// 線描画
	virtual void DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color) override;
	// カプセル描画
	void DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color) override;
	// ---リソース削除関数---

	// モデル素材削除
	virtual void DeleteModel(int _handle) override;
	// 画像素材削除
	virtual void DeleteGraph(int _handle) override;
	// デストラクタ
	~RaylibRenderer() = default;

private:
	bool useZDepth{ true };
	bool writeZDepth{ true };

	int nextResourceHandle{ 0 };

	std::unordered_map<int, Model> modelMap;
	std::unordered_map<int, Texture2D> textureMap;

	RlCamera3D raylibCamera{
		RlVector3{0.0f, 0.0f, -10.0f}, // position
		RlVector3{0.0f, 0.0f,  0.0f},  // target
		RlVector3{0.0f, 1.0f,  0.0f},  // up
		45.0f,                         // vertical FOV
		CAMERA_PERSPECTIVE
	};
};
