#pragma once

#include <limits>

#include "IRenderer.h"

#include "ResourceStorage.h"

class DxlibRenderer : public IRenderer
{
private:
	// カメラ関連
	// カメラセット
	void SetCamera(const Matrix4x4& _view, float _near, float _far) override;
	// ---ダブルバッファリング用---
	
	// 垂直同期信号を待つ
	int ClearDrawScreen() override;
	// ダブルバッファリングの反転
	int ScreenFlip() override;
	// --- 設定 ---

	// ZDepth使う
	int SetUseZDepth(bool _flag) override;
	// ZDepth書き込み
	int SetWriteZDepth(bool _flag) override;

	// --- モデル情報セット系 ---
	
	// 行列セット
	void ModelSetMatrix(ModelHandle _handle, const Matrix4x4& _mat) override;
	// テクスチャをShaderに渡す。
	void BindTexture(TextureHandle _handle, uint32_t _slot) override;
	// キューブテクスチャをShaderに渡す。
	void BindCubeTexture(CubeTextureHandle _handle, uint32_t _slot) override;

	// ---描画関数---
	
	// モデル描画
	void DrawModel(ModelHandle _handle) override;
	// 画像描画
	void DrawTexture(TextureHandle _handle, const Vector2& _pos, bool _transFlag) override;
	// 球描画
	void DrawSphere(const Vector3& _pos, float _radius, const Color& _color) override;
	// メッシュ球描画
	void DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color) override;
	// Box描画
	void DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color) override;
	// 線描画
	void DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color) override;
	// カプセル描画
	void DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color);
	// 円錐描画
	void DrawCone(const Vector3& _topPos, const Vector3& _bottomPos, float _radius, int _division, const Color& _color) override;

	// --- アニメーション関連 ---

	// モデルのスケルトンデータのロード
	SkeletonHandle LoadSkeletonData(ModelHandle _handle) override;
	// モデルのスケルトンデータの取得
	const SkeletonData* GetSkeletonData(SkeletonHandle _handle) override;
	// 現在のスケルトンのポーズ情報の取得
	bool GetCurrentPose(ModelHandle _handle, PoseBuffer& _output) override;
	// モデルに、ポーズを適応する
	bool ApplyPose(ModelHandle _handle, const PoseBuffer& _pose) override;
	// アニメーションの適用
	AnimationHandle AttachAnimation(ModelHandle _handle, int _animIndex) override;
	// 適用中のアニメーションの時間を設定する
	void SetAnimationTime(ModelHandle _model, AnimationHandle _handle, float _timeSeconds) override;
	// アニメーションの解除
	void DetachAnimation(ModelHandle _model, AnimationHandle _handle) override;
	// アニメーション指定時間の姿勢取得
	bool GetAttachAnimFramePose(ModelHandle _model, AnimationHandle _handle, PoseBuffer& _output) override;
	// アニメーションの総時間を取得
	float GetAnimationDuration(ModelHandle _model, AnimationHandle _handle) override;
	// モデルのアニメーション数を取得
	int GetAnimationCount(ModelHandle _model) override;
	// アニメーション番号に対しての名前を取得
	std::string GetAnimationName(ModelHandle _model, int _animIndex) override;

	// ---読み込み関数---

	// モデルの読み込み
	ModelHandle LoadModel(const std::string& _fileName) override;
	// モデル複製
	ModelHandle DuplicateModel(ModelHandle _handle) override;
	// 画像の読み込み
	TextureHandle LoadTexture(const std::string& _fileName) override;
	// CubeTextureの読み込み
	CubeTextureHandle LoadCubeTexture(const std::string& _fileName) override;
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
	void LoadDivTexture(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf) override;

	// ---リソース削除関数---

	// モデル素材削除
	void DeleteModel(ModelHandle _handle) override;
	// 画像素材削除
	void DeleteTexture(TextureHandle _handle) override;
	// キューブテクスチャ削除
	void DeleteCubeTexture(CubeTextureHandle _handle) override;

	// すべてのリソースを削除
	void DeleteAll() override;
private:
	std::string TCHARToUTF8(const wchar_t* text);
private:
	static constexpr uint32_t INVALID_BONE{ UINT32_MAX };
	static constexpr int FBX_ANIMATION_FPS{ 30 };

private:
	/*
		ライブラリによるハンドルの型の違いをResourceStorageで受け止める
	*/
	ResourceStorage<ModelHandle, int> modelStorage;
	ResourceStorage<TextureHandle, int> textureStorage;
	ResourceStorage<CubeTextureHandle, int> cubeTextureStorage;
	ResourceStorage<SkeletonHandle, SkeletonData> skeletonStorage;
	ResourceStorage<AnimationHandle, int> animationStorage;
};
