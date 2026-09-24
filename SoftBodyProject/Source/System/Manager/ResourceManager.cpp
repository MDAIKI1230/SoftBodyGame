#include "ServiceLocator.h"

#include "ResourceManager.h"

// --- 外に公開する関数の実装部分 ---

// 3Dモデルロード
bool ResourceManager::LoadModelImpl(std::filesystem::path _path)
{
	// ファイル名だけをキーとして扱う
	const auto key{ _path.filename() };

	ModelHandle modelHandle{ ServiceLocator::GetRenderer()->LoadModel(_path.string()) };

	modelMasters[key] = modelHandle;

	// スケルトンの取得も済ませて置く
	SkeletonHandle skeletonHandle{ ServiceLocator::GetRenderer()->LoadSkeletonData(modelMasters[key]) };

	if (skeletonHandle.IsValid())
	{
		modelToSkeleton[modelHandle] = skeletonHandle;
	}

	// アニメーションを名前から行けるようにしとこう
	int animeCount{ ServiceLocator::GetRenderer()->GetAnimationCount(modelHandle) };

	for (int animIndex{ 0 }; animIndex < animeCount; animIndex++)
	{
		std::string animName{ ServiceLocator::GetRenderer()->GetAnimationName(modelHandle,animIndex) };
		animationIndecies[animName] = animIndex;
	}

	return modelMasters[key].GetGeneration() != 0;
}
// テクスチャロード
bool ResourceManager::LoadTextureImpl(std::filesystem::path _path)
{
	// ファイル名だけをキーとして扱う
	const auto key{ _path.filename() };

	textureMasters[key] = ServiceLocator::GetRenderer()->LoadTexture(_path.string());

	return textureMasters[key].GetGeneration() != 0;
}
// キューブテクスチャロード
bool ResourceManager::LoadCubeTextureImpl(std::filesystem::path _path)
{
	// ファイル名だけをキーとして扱う
	const auto key{ _path.filename() };

	cubeTextureMasters[key] = ServiceLocator::GetRenderer()->LoadCubeTexture(_path.string());

	return cubeTextureMasters[key].GetGeneration() != 0;
}
// 頂点シェーダシェーダ
bool ResourceManager::LoadVertexShaderImpl(std::filesystem::path _path)
{
	// ファイル名だけをキーとして扱う
	const auto key{ _path.filename() };

	vertexShaderMasters[key] = ServiceLocator::GetGPUConnecter()->LoadVertexShader(_path.string());

	return vertexShaderMasters[key].GetGeneration() != 0;
}
// ピクセルシェーダシェーダ
bool ResourceManager::LoadPixelShaderImpl(std::filesystem::path _path)
{
	// ファイル名だけをキーとして扱う
	const auto key{ _path.filename() };

	pixelShaderMasters[key] = ServiceLocator::GetGPUConnecter()->LoadPixelShader(_path.string());

	return pixelShaderMasters[key].GetGeneration() != 0;
}

// 音源ロード
bool ResourceManager::LoadSoundImpl(std::filesystem::path _path)
{
	const auto key{ _path.filename() };

	if (soundMasters.contains(key))
	{
		return true;
	}

	ISound* sound{ ServiceLocator::GetSound() };

	if (sound == nullptr)
	{
		return false;
	}

	SoundHandle handle{ sound->LoadSound(_path.string()) };

	if (!handle.IsValid())
	{
		return false;
	}

	soundMasters[key] = handle;

	return true;
}

ShaderConstantBufferHandle ResourceManager::CreateConstantBufferImpl(uint32_t _size)
{
	return ServiceLocator::GetGPUConnecter()->CreateConstantBuffer(_size);
}

// 3Dモデル取得
ModelHandle ResourceManager::GetModelImpl(std::filesystem::path _path)
{
	// あるかチェックして無かったら無効値を返す
	if (modelMasters.contains(_path))
	{
		ModelHandle originalHandle{ modelMasters[_path] };
		ModelHandle handle{ ServiceLocator::GetRenderer()->DuplicateModel(originalHandle) };
		modelToSkeleton[handle] = modelToSkeleton[originalHandle];
		return handle;
	}
	
	return {};
}
// テクスチャ取得
TextureHandle ResourceManager::GetTextureImpl(std::filesystem::path _path)
{
	if (textureMasters.contains(_path))
	{
		return textureMasters[_path];
	}
	
	return {};
}
// キューブテクスチャ取得
CubeTextureHandle ResourceManager::GetCubeTextureImpl(std::filesystem::path _path)
{
	if (cubeTextureMasters.contains(_path))
	{
		return cubeTextureMasters[_path];
	}

	return {};
}
// 頂点シェーダ取得
VertexShaderHandle ResourceManager::GetVertexShaderImpl(std::filesystem::path _path)
{
	if (vertexShaderMasters.contains(_path))
	{
		return vertexShaderMasters[_path];
	}
	
	return {};
}
// ピクセルシェーダ取得
PixelShaderHandle ResourceManager::GetPixelShaderImpl(std::filesystem::path _path)
{
	if (pixelShaderMasters.contains(_path))
	{
		return pixelShaderMasters[_path];
	}
	
	return {};
}

// モデルのハンドルからスケルトンのデータを取得する
const SkeletonData* ResourceManager::GetSkeletonDataImpl(ModelHandle _model)
{
	if (modelToSkeleton.contains(_model))
	{
		return ServiceLocator::GetRenderer()->GetSkeletonData(modelToSkeleton[_model]);
	}

	return nullptr;
}

// モデルの現在のポーズを取得する
bool ResourceManager::GetPoseImpl(ModelHandle _model, PoseBuffer& _output)
{
	// 無効IDなら失敗を渡す
	if(!_model.IsValid())
	{
		return false;
	}

	return ServiceLocator::GetRenderer()->GetCurrentPose(_model, _output);
}

bool ResourceManager::GetPoseImpl(ModelHandle _model, AnimationHandle _anim, PoseBuffer& _output)
{
	// 無効IDなら失敗を渡す
	if (!_model.IsValid() || !_anim.IsValid())
	{
		return false;
	}

	return ServiceLocator::GetRenderer()->GetAttachAnimFramePose(_model, _anim, _output);
}

// アタッチされてるアニメーションの総再生時間を取得する
float ResourceManager::GetAnimTotalTimeImpl(ModelHandle _model, AnimationHandle _anim)
{
	return ServiceLocator::GetRenderer()->GetAnimationDuration(_model, _anim);
}

// モデルにTRS列を適応する
void ResourceManager::SetMatrixImpl(ModelHandle _model, const Matrix4x4& _mat)
{
	// 無効IDなら失敗を渡す
	if (!_model.IsValid())
	{
		return;
	}

	ServiceLocator::GetRenderer()->ModelSetMatrix(_model, _mat);
}

// 音源取得
SoundHandle ResourceManager::GetSoundImpl(std::filesystem::path _path)
{
	const auto key{ _path.filename() };

	if (soundMasters.contains(key))
	{
		return soundMasters.at(key);
	}

	return {};
}

// モデルにポーズを適応させる(アニメーションを踏まえない)
bool ResourceManager::ApplyPoseImpl(ModelHandle _model, const PoseBuffer& _pose)
{
	// 無効IDなら失敗を渡す
	if (!_model.IsValid())
	{
		return false;
	}

	return ServiceLocator::GetRenderer()->ApplyPose(_model, _pose);
}

// モデルにアニメーションを適用させる
AnimationHandle ResourceManager::AttachAnimationImpl(ModelHandle _handle, std::string _animName)
{
	if (animationIndecies.contains(_animName))
	{
		return ServiceLocator::GetRenderer()->AttachAnimation(_handle, animationIndecies.at(_animName));
	}

	return {};
}

// モデルに適用させてるアニメーションに時間を設定する
void ResourceManager::SetAnimationTimeImpl(ModelHandle _model, AnimationHandle _handle, float _time)
{
	ServiceLocator::GetRenderer()->SetAnimationTime(_model, _handle, _time);
}

// モデルに適用させてるアニメーションを解除
void ResourceManager::DetachAnimationImpl(ModelHandle _model, AnimationHandle _handle)
{
	ServiceLocator::GetRenderer()->DetachAnimation(_model, _handle);
}

// 3Dモデル共有リソース破棄
void ResourceManager::DestroyModelImpl(ModelHandle _handle)
{
	ServiceLocator::GetRenderer()->DeleteModel(_handle);
}

// 3Dモデル原本データ破棄
void ResourceManager::UnLoadModelImpl(std::filesystem::path _path)
{
	// 複製まで消す
	for (auto handle : modelSharedResource[_path])
	{
		ServiceLocator::GetRenderer()->DeleteModel(handle);
		modelToSkeleton.erase(handle);
	}

	ModelHandle handle{ modelMasters[_path] };

	// アニメーションを名前から行けるようにしとこう
	int animeCount{ ServiceLocator::GetRenderer()->GetAnimationCount(handle) };

	for (int animIndex{ 0 }; animIndex < animeCount; animIndex++)
	{
		std::string animName{ ServiceLocator::GetRenderer()->GetAnimationName(handle,animIndex) };
		animationIndecies.erase(animName);
	}

	// スケルトンも消す
	modelToSkeleton.erase(handle);

	ServiceLocator::GetRenderer()->DeleteModel(handle);

	modelMasters.erase(_path);
	modelSharedResource.erase(_path);
}
// テクスチャ原本データ破棄
void ResourceManager::UnLoadTextureImpl(std::filesystem::path _path)
{
	ServiceLocator::GetRenderer()->DeleteTexture(textureMasters[_path]);

	textureMasters.erase(_path);
}
// キューブテクスチャ原本データ破棄
void ResourceManager::UnLoadCubeTextureImpl(std::filesystem::path _path)
{
	ServiceLocator::GetRenderer()->DeleteCubeTexture(cubeTextureMasters[_path]);

	cubeTextureMasters.erase(_path);
}
// 頂点シェーダ原本データ破棄
void ResourceManager::UnLoadVertexShaderImpl(std::filesystem::path _path)
{
	ServiceLocator::GetGPUConnecter()->DestroyVertexShader(vertexShaderMasters[_path]);

	vertexShaderMasters.erase(_path);
}
// ピクセルシェーダ原本データ破棄
void ResourceManager::UnLoadPixelShaderImpl(std::filesystem::path _path)
{
	ServiceLocator::GetGPUConnecter()->DestroyPixelShader(pixelShaderMasters[_path]);

	pixelShaderMasters.erase(_path);
}

// 定数バッファ破棄
void ResourceManager::UnLoadConstantBufferImpl(ShaderConstantBufferHandle _handle)
{
	ServiceLocator::GetGPUConnecter()->DestroyConstantBuffer(_handle);
}

// 音源破棄
void ResourceManager::UnLoadSoundImpl(std::filesystem::path _path)
{
	const auto key{ _path.filename() };
	auto it{ soundMasters.find(key) };

	if (it == soundMasters.end())
	{
		return;
	}

	ISound* sound{ ServiceLocator::GetSound() };

	if (sound == nullptr)
	{
		return;
	}

	sound->DeleteSound(it->second);
	soundMasters.erase(it);
}

// すべてのリソースを破棄する
void ResourceManager::UnLoadAllImpl()
{
	ISound* sound{ ServiceLocator::GetSound() };

	if (sound != nullptr)
	{
		for (const auto& [path, handle] : soundMasters)
		{
			sound->DeleteSound(handle);
		}
	}

	soundMasters.clear();

	ServiceLocator::GetGPUConnecter()->DeleteAll();
	ServiceLocator::GetRenderer()->DeleteAll();
}
