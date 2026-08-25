#pragma once

#include <filesystem>
#include <unordered_map>

#include "ResourceStorage.h"

#include "ModelHandle.h"
#include "TextureHandle.h"
#include "CubeTextureHandle.h"
#include "VertexShaderHandle.h"
#include "PixelShaderHandle.h"

class ResourceManager
{
public:
	/// <summary>
	/// 3Dモデルロード
	/// </summary>
	/// <param name="_path">フルパス</param>
	bool LoadModel(std::filesystem::path _path);
	/// <summary>
	/// テクスチャロード
	/// </summary>
	/// <param name="_path">フルパス</param>
	bool LoadTexture(std::filesystem::path _path);
	/// <summary>
	/// キューブテクスチャロード
	/// </summary>
	/// <param name="_path">フルパス</param>
	bool LoadCubeTexture(std::filesystem::path _path);
	/// <summary>
	/// 頂点シェーダシェーダ
	/// </summary>
	/// <param name="_path">フルパス</param>
	bool LoadVertexShader(std::filesystem::path _path);
	/// <summary>
	/// ピクセルシェーダシェーダ
	/// </summary>
	/// <param name="_path">フルパス</param>
	bool LoadPixelShader(std::filesystem::path _path);

	/// <summary>
	/// 3Dモデル取得(複製データなので、Destroy関数の削除を忘れず)
	/// </summary>
	/// <param name="_path">データの名前</param>
	ModelHandle GetModel(std::filesystem::path _path);
	/// <summary>
	/// テクスチャ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	TextureHandle GetTexture(std::filesystem::path _path);
	/// <summary>
	/// キューブテクスチャ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	CubeTextureHandle GetCubeTexture(std::filesystem::path _path);
	/// <summary>
	/// 頂点シェーダ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	VertexShaderHandle GetVertexShader(std::filesystem::path _path);
	/// <summary>
	/// ピクセルシェーダ取得
	/// </summary>
	/// <param name="_path">データの名前</param>
	PixelShaderHandle GetPixelShader(std::filesystem::path _path);

	/// <summary>
	/// 3Dモデル共有リソース破棄
	/// </summary>
	/// <param name="_handle">ハンドル</param>
	void DestroyModel(ModelHandle _handle);

	/// <summary>
	/// 3Dモデル原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	void UnLoadModel(std::filesystem::path _path);
	/// <summary>
	/// テクスチャ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	void UnLoadTexture(std::filesystem::path _path);
	/// <summary>
	/// キューブテクスチャ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	void UnLoadCubeTexture(std::filesystem::path _path);
	/// <summary>
	/// 頂点シェーダ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	void UnLoadVertexShader(std::filesystem::path _path);
	/// <summary>
	/// ピクセルシェーダ原本データ破棄
	/// </summary>
	/// <param name="_path">データの名前</param>
	void UnLoadPixelShader(std::filesystem::path _path);
private:
	// --- 原本データ ---

	std::unordered_map <std::filesystem::path, ModelHandle> modelMasters;
	std::unordered_map <std::filesystem::path, TextureHandle> textureMasters;
	std::unordered_map <std::filesystem::path, CubeTextureHandle> cubeTextureMasters;
	std::unordered_map <std::filesystem::path, VertexShaderHandle> vertexShaderMasters;
	std::unordered_map <std::filesystem::path, PixelShaderHandle> pixelShaderMasters;


	// --- 共有データ ---

	std::unordered_map<std::filesystem::path, std::vector<ModelHandle>> modelSharedResource;
};
