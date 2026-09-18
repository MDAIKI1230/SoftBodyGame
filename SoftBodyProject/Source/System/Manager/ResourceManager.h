#pragma once

#include <filesystem>
#include <unordered_map>

#include "ServiceLocator.h"

#include "SingleTonMacros.h"

#include "SkeletonData.h"
#include "Buffer/PoseBuffer.h"

#include "ModelHandle.h"
#include "TextureHandle.h"
#include "CubeTextureHandle.h"
#include "VertexShaderHandle.h"
#include "PixelShaderHandle.h"
#include "ShaderConstantBufferHandle.h"


/*
	リソース全般の管理を担当する
	データのロード/破棄/変更/取得
	の責任を持つ
*/
class ResourceManager
{
public:
	/// <summary>
	/// 3Dモデルロード
	/// </summary>
	/// <param name="_path">フルパス</param>
	static bool LoadModel(std::filesystem::path _path)
	SingletonPublicFunction(bool, LoadModel, (std::filesystem::path _path), (_path))
	/// <summary>
	/// テクスチャロード
	/// </summary>
	/// <param name="_path">フルパス</param>
	static bool LoadTexture(std::filesystem::path _path)
	SingletonPublicFunction(bool, LoadTexture, (std::filesystem::path _path), (_path))
	/// <summary>
	/// キューブテクスチャロード
	/// </summary>
	/// <param name="_path">フルパス</param>
	static bool LoadCubeTexture(std::filesystem::path _path)
	SingletonPublicFunction(bool, LoadCubeTexture, (std::filesystem::path _path), (_path))
	/// <summary>
	/// 頂点シェーダシェーダ
	/// </summary>
	/// <param name="_path">フルパス</param>
	static bool LoadVertexShader(std::filesystem::path _path)
	SingletonPublicFunction(bool, LoadVertexShader, (std::filesystem::path _path), (_path))
	/// <summary>
	/// ピクセルシェーダシェーダ
	/// </summary>
	/// <param name="_path">フルパス</param>
	static bool LoadPixelShader(std::filesystem::path _path)
	SingletonPublicFunction(bool, LoadPixelShader, (std::filesystem::path _path), (_path))

	/// <summary>
	/// 定数バッファ作成
	/// </summary>
	/// <param name="_size">メモリサイズ</param>
	static ShaderConstantBufferHandle CreateConstantBuffer(uint32_t _size)
	SingletonPublicFunction(ShaderConstantBufferHandle, CreateConstantBuffer, (uint32_t _size), (_size))

	/// <summary>
	/// 3Dモデル取得(複製データなので、Destroy関数の削除を忘れず)
	/// </summary>
	/// <param name="_path">データの名前</param>
	static ModelHandle GetModel(std::filesystem::path _path)
	SingletonPublicFunction(ModelHandle, GetModel, (std::filesystem::path _path), (_path))
	/// <summary>
	/// テクスチャ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	static TextureHandle GetTexture(std::filesystem::path _path)
	SingletonPublicFunction(TextureHandle, GetTexture, (std::filesystem::path _path), (_path))
	/// <summary>
	/// キューブテクスチャ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	static CubeTextureHandle GetCubeTexture(std::filesystem::path _path)
	SingletonPublicFunction(CubeTextureHandle, GetCubeTexture, (std::filesystem::path _path), (_path))
	/// <summary>
	/// 頂点シェーダ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	static VertexShaderHandle GetVertexShader(std::filesystem::path _path)
	SingletonPublicFunction(VertexShaderHandle, GetVertexShader, (std::filesystem::path _path), (_path))
	/// <summary>
	/// ピクセルシェーダ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	static PixelShaderHandle GetPixelShader(std::filesystem::path _path)
	SingletonPublicFunction(PixelShaderHandle, GetPixelShader, (std::filesystem::path _path), (_path))
	/// <summary>
	/// モデルのハンドルからスケルトンのデータを取得する
	/// </summary>
	/// <param name="_path">モデルハンドル</param>
	static const SkeletonData* GetSkeletonData(ModelHandle _model)
	SingletonPublicFunction(const SkeletonData*, GetSkeletonData, (ModelHandle _model), (_model))
	/// <summary>
	/// モデルの現在のポーズを取得する
	/// </summary>
	/// <param name="_model">モデルハンドル</param>
	/// <returns>成功したか</returns>
	static bool GetPose(ModelHandle _model, PoseBuffer& _output)
	SingletonPublicFunction(bool, GetPose, (ModelHandle _model, PoseBuffer& _output), (_model, _output))

	/// <summary>
	/// モデルにTRS列を適用する
	/// </summary>
	/// <param name="_model">モデルハンドル</param>
	/// <param name="_pose">ポーズ</param>
	static void SetMatrix(ModelHandle _model, const Matrix4x4& _mat)
	SingletonPublicFunction(void, SetMatrix, (ModelHandle _model, const Matrix4x4& _mat), (_model, _mat))
	/// <summary>
	/// モデルにポーズを適用させる(アニメーションを踏まえない)
	/// </summary>
	/// <param name="_model">モデルハンドル</param>
	/// <param name="_pose">ポーズ</param>
	/// <returns>成功したか</returns>
	static bool ApplyPose(ModelHandle _model, const PoseBuffer& _pose)
	SingletonPublicFunction(bool, ApplyPose, (ModelHandle _model, const PoseBuffer& _pose), (_model, _pose))
	/// <summary>
	/// モデルにアニメーションを適用させる
	/// </summary>
	/// <param name="_handle">モデルハンドル</param>
	/// <param name="_animIndex">アニメーション番号</param>
	/// <returns>成功したか</returns>
	static AnimationHandle AttachAnimation(ModelHandle _handle, std::string _animName)
	SingletonPublicFunction(AnimationHandle, AttachAnimation, (ModelHandle _handle, std::string _animName), (_handle, _animName))
	/// <summary>
	/// モデルに適用させてるアニメーションに時間を設定する
	/// </summary>
	/// <param name="_model">モデルハンドル</param>
	/// <param name="_handle">アニメーションハンドル</param>
	static void SetAnimationTime(ModelHandle _model, AnimationHandle _handle, float _time)
	SingletonPublicFunction(void, SetAnimationTime, (ModelHandle _model, AnimationHandle _handle, float _time), (_model, _handle, _time))
	/// <summary>
	/// モデルに適用させてるアニメーションを解除
	/// </summary>
	/// <param name="_model">モデルハンドル</param>
	/// <param name="_handle">アニメーションハンドル</param>
	static void DetachAnimation(ModelHandle _model, AnimationHandle _handle)
	SingletonPublicFunction(void, DetachAnimation, (ModelHandle _model, AnimationHandle _handle), (_model, _handle))
	/// <summary>
	/// 定数バッファに値を渡す
	/// </summary>
	/// <param name="_handle">定数バッファハンドル</param>
	/// <param name="_value">値のVoidポインタ</param>
	template<class T>
	static bool SetConstantBufferValue(ShaderConstantBufferHandle _handle, void* _value)
	SingletonPublicTemplateFunction(bool, template<class T>, T, SetConstantBufferValue, (ShaderConstantBufferHandle _handle, void* _value), (_handle, _value))

	/// <summary>
	/// 3Dモデル共有リソース破棄
	/// </summary>
	/// <param name="_handle">ハンドル</param>
	static void DestroyModel(ModelHandle _handle)
	SingletonPublicFunction(void, DestroyModel, (ModelHandle _handle), (_handle))

	/// <summary>
	/// 3Dモデル原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	static void UnLoadModel(std::filesystem::path _path)
	SingletonPublicFunction(void, UnLoadModel, (std::filesystem::path _path), (_path))
	/// <summary>
	/// テクスチャ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	static void UnLoadTexture(std::filesystem::path _path)
	SingletonPublicFunction(void, UnLoadTexture, (std::filesystem::path _path), (_path))
	/// <summary>
	/// キューブテクスチャ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	static void UnLoadCubeTexture(std::filesystem::path _path)
	SingletonPublicFunction(void, UnLoadCubeTexture, (std::filesystem::path _path), (_path))
	/// <summary>
	/// 頂点シェーダ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	static void UnLoadVertexShader(std::filesystem::path _path)
	SingletonPublicFunction(void, UnLoadVertexShader, (std::filesystem::path _path), (_path))
	/// <summary>
	/// ピクセルシェーダ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	static void UnLoadPixelShader(std::filesystem::path _path)
	SingletonPublicFunction(void, UnLoadPixelShader, (std::filesystem::path _path), (_path))
	/// <summary>
	/// 定数バッファ破棄
	/// </summary>
	/// <param name="_handle">定数バッファハンドル</param>
	static void UnLoadConstantBuffer(ShaderConstantBufferHandle _handle)
	SingletonPublicFunction(void, UnLoadConstantBuffer, (ShaderConstantBufferHandle _handle), (_handle))
	/// <summary>
	/// すべてのリソースを破棄する
	/// </summary>
	static void UnLoadAll()
	SingletonPublicFunction(void, UnLoadAll, (), ())
private:
	// シングルトン
	// コンストラクタ
	ResourceManager() = default;
	static ResourceManager& Instance()
	{
		static ResourceManager instance;

		return instance;
	}
private:
	// --- 原本データ ---

	std::unordered_map <std::filesystem::path, ModelHandle> modelMasters;
	std::unordered_map <std::filesystem::path, TextureHandle> textureMasters;
	std::unordered_map <std::filesystem::path, CubeTextureHandle> cubeTextureMasters;
	std::unordered_map <std::filesystem::path, VertexShaderHandle> vertexShaderMasters;
	std::unordered_map <std::filesystem::path, PixelShaderHandle> pixelShaderMasters;
	std::unordered_map <std::filesystem::path, int> animationIndecies;

	std::unordered_map <ModelHandle, SkeletonHandle> modelToSkeleton;


	// --- 共有データ ---

	std::unordered_map<std::filesystem::path, std::vector<ModelHandle>> modelSharedResource;
};

/*
	テンプレートなのでヘッダ何に書いてる
*/

// 定数バッファに値を渡す
template<class T>
bool ResourceManager::SetConstantBufferValueImpl(ShaderConstantBufferHandle _handle, void* _value)
{
	if (!_handle.IsValid())
	{
		return false;
	}

	// 定数バッファに値渡して上げる
	void* pBuffer{ ServiceLocator::GetGPUConnecter()->GetConstantBufferAddress(_handle) };

	memcpy(pBuffer, _value, sizeof(T));

	ServiceLocator::GetGPUConnecter()->UpdateConstantBuffer(_handle, _value, sizeof(T));

	return true;
}
