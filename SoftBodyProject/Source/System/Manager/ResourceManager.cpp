#include "ServiceLocator.h"

#include "ResourceManager.h"

// 3Dモデルロード
bool ResourceManager::LoadModel(std::filesystem::path _path)
{
	modelMasters[_path.filename()] = ServiceLocator::GetRenderer()->LoadModel(_path.string());

	return modelMasters[_path.filename()].GetGeneration() != 0;
}
// テクスチャロード
bool ResourceManager::LoadTexture(std::filesystem::path _path)
{
	textureMasters[_path.filename()] = ServiceLocator::GetRenderer()->LoadTexture(_path.string());

	return textureMasters[_path.filename()].GetGeneration() != 0;
}
// キューブテクスチャロード
bool ResourceManager::LoadCubeTexture(std::filesystem::path _path)
{
	cubeTextureMasters[_path.filename()] = ServiceLocator::GetRenderer()->LoadCubeTexture(_path.string());

	return cubeTextureMasters[_path.filename()].GetGeneration() != 0;
}
// 頂点シェーダシェーダ
bool ResourceManager::LoadVertexShader(std::filesystem::path _path)
{
	vertexShaderMasters[_path.filename()] = ServiceLocator::GetGPUConnecter()->LoadVertexShader(_path.string());

	return vertexShaderMasters[_path.filename()].GetGeneration() != 0;
}
// ピクセルシェーダシェーダ
bool ResourceManager::LoadPixelShader(std::filesystem::path _path)
{
	pixelShaderMasters[_path.filename()] = ServiceLocator::GetGPUConnecter()->LoadPixelShader(_path.string());

	return pixelShaderMasters[_path.filename()].GetGeneration() != 0;
}

// 3Dモデル取得
ModelHandle ResourceManager::GetModel(std::filesystem::path _path)
{
	return ServiceLocator::GetRenderer()->DuplicateModel(modelMasters[_path]);
}
// テクスチャ取得
TextureHandle ResourceManager::GetTexture(std::filesystem::path _path)
{
	return textureMasters[_path];
}
// キューブテクスチャ取得
CubeTextureHandle ResourceManager::GetCubeTexture(std::filesystem::path _path)
{
	return cubeTextureMasters[_path];
}
// 頂点シェーダ取得
VertexShaderHandle ResourceManager::GetVertexShader(std::filesystem::path _path)
{
	return vertexShaderMasters[_path];
}
// ピクセルシェーダ取得
PixelShaderHandle ResourceManager::GetPixelShader(std::filesystem::path _path)
{
	return pixelShaderMasters[_path];
}

// 3Dモデル共有リソース破棄
void ResourceManager::DestroyModel(ModelHandle _handle)
{
	ServiceLocator::GetRenderer()->DeleteModel(_handle);
}

// 3Dモデル原本データ破棄
void ResourceManager::UnLoadModel(std::filesystem::path _path)
{
	for (auto handle : modelSharedResource[_path])
	{
		ServiceLocator::GetRenderer()->DeleteModel(handle);
	}

	ServiceLocator::GetRenderer()->DeleteModel(modelMasters[_path]);

	modelMasters.erase(_path);
	modelSharedResource.erase(_path);
}
// テクスチャ原本データ破棄
void ResourceManager::UnLoadTexture(std::filesystem::path _path)
{
	ServiceLocator::GetRenderer()->DeleteTexture(textureMasters[_path]);

	textureMasters.erase(_path);
}
// キューブテクスチャ原本データ破棄
void ResourceManager::UnLoadCubeTexture(std::filesystem::path _path)
{
	ServiceLocator::GetRenderer()->DeleteCubeTexture(cubeTextureMasters[_path]);

	cubeTextureMasters.erase(_path);
}
// 頂点シェーダ原本データ破棄
void ResourceManager::UnLoadVertexShader(std::filesystem::path _path)
{
	ServiceLocator::GetGPUConnecter()->DestroyVertexShader(vertexShaderMasters[_path]);

	vertexShaderMasters.erase(_path);
}
// ピクセルシェーダ原本データ破棄
void ResourceManager::UnLoadPixelShader(std::filesystem::path _path)
{
	ServiceLocator::GetGPUConnecter()->DestroyPixelShader(pixelShaderMasters[_path]);

	pixelShaderMasters.erase(_path);
}
