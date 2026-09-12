#include <DxLib.h>

#include "DxlibConvert.h"

#include "DxlibRenderer.h"

void DxlibRenderer::SetCamera(const Matrix4x4& _view, float _near, float _far)
{
	// 位置と見る点を決める
	DxLib::SetCameraViewMatrix(ToDxlib(_view));
	DxLib::SetCameraNearFar(_near, _far);
}

int DxlibRenderer::ClearDrawScreen()
{
	return DxLib::ClearDrawScreen();
}

int DxlibRenderer::ScreenFlip()
{
	return DxLib::ScreenFlip();
}

// ZDepth使う
int DxlibRenderer::SetUseZDepth(bool _flag)
{
	return DxLib::SetUseZBuffer3D(_flag);
}

// ZDepth書き込み
int DxlibRenderer::SetWriteZDepth(bool _flag)
{
	return DxLib::SetWriteZBuffer3D(_flag);
}

// モデルの読み込み
ModelHandle DxlibRenderer::LoadModel(const std::string& _fileName)
{
	int nativeHandle{ DxLib::MV1LoadModel(std::wstring(_fileName.begin(), _fileName.end()).c_str()) };

	if (nativeHandle == -1)
	{
		return {};
	}

	return modelStorage.Add(nativeHandle);
}

// モデル複製
ModelHandle DxlibRenderer::DuplicateModel(ModelHandle _handle)
{
	int nativeHandle;

	if (modelStorage.TryGet(_handle, nativeHandle))
	{
		return modelStorage.Add(DxLib::MV1DuplicateModel(nativeHandle));
	}

	return{};
}

// 画像の読み込み
TextureHandle DxlibRenderer::LoadTexture(const std::string& _fileName)
{
	int nativeHandle{ DxLib::LoadGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str()) };

	if (nativeHandle == -1)
	{
		return {};
	}

	return textureStorage.Add(nativeHandle);
}

// CubeTextureの読み込み
CubeTextureHandle DxlibRenderer::LoadCubeTexture(const std::string& _fileName)
{
	const std::wstring filePath(_fileName.begin(),_fileName.end());

	DxLib::SetCubeMapTextureCreateFlag(TRUE);

	int handle{ DxLib::LoadGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str()) };

	// 元に戻す
	DxLib::SetCubeMapTextureCreateFlag(FALSE);

	return cubeTextureStorage.Add(handle);
}

// 画像の分割読み込み
void DxlibRenderer::LoadDivTexture(const std::string& _fileName, int _allNum, int _xNum, int _yNum, int _xSize, int _ySize, int* _handleBuf)
{
	DxLib::LoadDivGraph(std::wstring(_fileName.begin(), _fileName.end()).c_str(), _allNum, _xNum, _yNum, _xSize, _ySize, _handleBuf);
}

// モデル情報セット系
// 行列セット
void DxlibRenderer::ModelSetMatrix(ModelHandle _handle, const Matrix4x4& _mat)
{
	int nativeHandle;

	if (modelStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::MV1SetMatrix(nativeHandle, ToDxlib(_mat));
	}
}

// ---描画関数---
// モデル描画
void DxlibRenderer::DrawModel(ModelHandle _handle)
{
	int nativeHandle;

	if (modelStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::MV1DrawModel(nativeHandle);
	}
}

// 画像描画
void DxlibRenderer::DrawTexture(TextureHandle _handle, const Vector2& _pos, bool _transFlag)
{
	int nativeHandle;

	if (textureStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::DrawGraph(static_cast<int>(_pos.x), static_cast<int>(_pos.y), nativeHandle, _transFlag);
	}
}

// 球描画
void DxlibRenderer::DrawSphere(const Vector3& _pos, float _radius, const Color& _color)
{
	DxLib::DrawSphere3D(
		ToDxlib(_pos), _radius, 8,
		ToDxlib(_color), ToDxlib(_color),
		true);
}

// メッシュ球描画
void DxlibRenderer::DrawSphereMesh(const Vector3& _pos, float _radius, const Color& _color)
{
	DxLib::DrawSphere3D(
		ToDxlib(_pos), _radius, 8,
		ToDxlib(_color), ToDxlib(_color),
		false);
}

// Box描画
void DxlibRenderer::DrawBox(const Matrix4x4& _mat, const Vector3& _size, const Color& _color)
{
	Vector3 halfSize = _size * 0.5f;

	// ローカル8頂点
	Vector3 point[8] = {
		{-halfSize.x, -halfSize.y, -halfSize.z},
		{ halfSize.x, -halfSize.y, -halfSize.z},
		{ halfSize.x,  halfSize.y, -halfSize.z},
		{-halfSize.x,  halfSize.y, -halfSize.z},

		{-halfSize.x, -halfSize.y,  halfSize.z},
		{ halfSize.x, -halfSize.y,  halfSize.z},
		{ halfSize.x,  halfSize.y,  halfSize.z},
		{-halfSize.x,  halfSize.y,  halfSize.z},
	};

	// ワールド変換
	for (int i = 0; i < 8; i++)
	{
		point[i] = _mat * point[i];
	}

	// エッジ12本
	auto L = [&](int a, int b)
		{
			DrawLine(point[a], point[b], _color);
		};

	// 下
	L(0, 1); L(1, 2); L(2, 3); L(3, 0);

	// 上
	L(4, 5); L(5, 6); L(6, 7); L(7, 4);

	// 横の線
	L(0, 4);
	L(1, 5);
	L(2, 6);
	L(3, 7);
}
// 線描画
void DxlibRenderer::DrawLine(const Vector3& _pos1, const Vector3& _pos2, const Color& _color)
{
	DxLib::DrawLine3D(ToDxlib(_pos1), ToDxlib(_pos2), ToDxlib(_color));
}
// カプセル描画
void DxlibRenderer::DrawCapsule(const Vector3& _pos1, const Vector3& _pos2, float _radius, const Color& _color)
{
	DxLib::DrawCapsule3D(ToDxlib(_pos1), ToDxlib(_pos2), _radius, 10, ToDxlib(_color), ToDxlib(_color), false);
}
// 円錐描画
void DxlibRenderer::DrawCone(const Vector3& _topPos, const Vector3& _bottomPos, float _radius, int _division, const Color& _color)
{
	DxLib::DrawCone3D(ToDxlib(_topPos), ToDxlib(_bottomPos),
		_radius, _division,
		ToDxlib(_color), ToDxlib(_color),
		false);
}

// --- アニメーション関連 ---

// モデルのスケルトンデータの取得
bool DxlibRenderer::GetSkeletonData(ModelHandle _handle, SkeletonData& _output)
{
	int nativeHandle;

	if (!modelStorage.TryGet(_handle, nativeHandle))
	{
		return false;
	}

	int frameCount{ MV1GetFrameNum(nativeHandle) };

	if (frameCount == -1)
	{
		return false;
	}

	_output.ReSize(frameCount);

	// 先に、親ボーンからのローカル座標を計算して保持しておく
	for (int frame{ 0 }; frame < frameCount; frame++)
	{
		_output.bindLocalMatrices[frame] = ToMDMath(MV1GetFrameBaseLocalMatrix(nativeHandle, frame));

		// 行列を分解して位置/回転/スケールを作る
		Transform::DecomposeTRS(
			_output.bindLocalMatrices[frame],
			_output.bindLocalPositions[frame],
			_output.bindLocalRotations[frame],
			_output.bindLocalScales[frame]);
	}

	for (int frame{ 0 }; frame < frameCount; frame++)
	{
		int parent{ MV1GetFrameParent(nativeHandle, frame) };

		// 無効値が-2らしいので-2の時は無効値にしておく
		if (parent == -2)
		{
			_output.parentIndices[frame] = INVALID_BONE;
			// 親がいないので上で求めた行列を等しくなる
			_output.bindModelMatrices[frame] = _output.bindLocalMatrices[frame];
		}
		else
		{
			_output.parentIndices[frame] = static_cast<uint32_t>(parent);
			// 親との計算をする
			_output.bindModelMatrices[frame] = _output.bindModelMatrices[parent] * _output.bindLocalMatrices[frame];
		}

		// 行列を分解して位置/回転/スケールを作る
		Transform::DecomposeTRS(
			_output.bindModelMatrices[frame],
			_output.bindModelPositions[frame],
			_output.bindModelRotations[frame],
			_output.bindModelScales[frame]);

		// 名前取得
		std::wstring name{ MV1GetFrameName(nativeHandle, frame) };

		if (name.empty())
		{
			continue;
		}

		_output.boneNames[frame] = std::string(name.begin(), name.end());
		_output.boneLookup[_output.boneNames[frame]] = frame;
	}

	return true;
}

// 現在のスケルトンのポーズ情報の取得
bool DxlibRenderer::GetCurrentPose(ModelHandle _handle, PoseBuffer& _output)
{
	int nativeHandle;

	if (!modelStorage.TryGet(_handle, nativeHandle))
	{
		return false;
	}

	int frameCount{ MV1GetFrameNum(nativeHandle) };

	if (frameCount == -1)
	{
		return false;
	}

	_output.ReSize(frameCount);

	for (int frame{ 0 }; frame < frameCount; frame++)
	{
		_output.localMatrices[frame] = ToMDMath(MV1GetFrameLocalMatrix(nativeHandle, frame));

		// 行列を分解して位置/回転/スケールを作る
		Transform::DecomposeTRS(
			_output.localMatrices[frame],
			_output.localPositions[frame],
			_output.localRotations[frame],
			_output.localScales[frame]);
	}

	// 全BoneのModel行列を作る
	for (uint32_t bone{ 0 }; bone < frameCount; bone++)
	{
		const Matrix4x4& localMatrix{ _output.localMatrices[bone] };

		int parent{ MV1GetFrameParent(nativeHandle, bone) };

		// 逆行列も用意する
		const Matrix4x4& inverseLocalMatrix{ MatGenerateFunc::InverseTRS(
			_output.localPositions[bone],
			_output.localRotations[bone],
			_output.localScales[bone]) };

		// 無効値が-2らしいので-2の時は無効値にしておく
		if (parent == -2)
		{
			// 親がいないので上で求めた行列と等しくなる
			_output.modelFromBoneMatrices[bone] = localMatrix;

			_output.boneFromModelMatrices[bone] = inverseLocalMatrix;
		}
		else
		{
			// 親との計算をする
			_output.modelFromBoneMatrices[bone] = _output.modelFromBoneMatrices[static_cast<uint32_t>(parent)] * localMatrix;

			_output.boneFromModelMatrices[bone] = inverseLocalMatrix * _output.boneFromModelMatrices[static_cast<uint32_t>(parent)];
		}
	}

	return true;
}

// モデルに、ポーズを適応する
bool DxlibRenderer::ApplyPose(ModelHandle _handle, const PoseBuffer& _pose)
{
	int nativeHandle;

	if (!modelStorage.TryGet(_handle, nativeHandle))
	{
		return false;
	}

	int frameCount{ MV1GetFrameNum(nativeHandle) };

	if (frameCount == -1)
	{
		return false;
	}

	for (int frame{ 0 }; frame < frameCount; ++frame)
	{
		MV1SetFrameUserLocalMatrix(nativeHandle, frame, ToDxlib(_pose.localMatrices[frame]));
	}

	return true;
}


// テクスチャをShaderに渡す。
void DxlibRenderer::BindTexture(TextureHandle _handle, uint32_t _slot)
{
	int nativeHandle;

	if (textureStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::SetUseTextureToShader(_slot, nativeHandle);
	}
}
// キューブテクスチャをShaderに渡す。
void DxlibRenderer::BindCubeTexture(CubeTextureHandle _handle, uint32_t _slot)
{
	int nativeHandle;

	if (cubeTextureStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::SetUseTextureToShader(_slot, nativeHandle);
	}
}

// モデル素材削除
void DxlibRenderer::DeleteModel(ModelHandle _handle)
{
	int nativeHandle;

	if (modelStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::MV1DeleteModel(nativeHandle);

		modelStorage.Remove(_handle);
	}
}

// 画像素材削除
void DxlibRenderer::DeleteTexture(TextureHandle _handle)
{
	int nativeHandle;

	if (textureStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::DeleteGraph(nativeHandle);

		textureStorage.Remove(_handle);
	}
}

// キューブテクスチャ削除
void DxlibRenderer::DeleteCubeTexture(CubeTextureHandle _handle)
{
	int nativeHandle;

	if (cubeTextureStorage.TryGet(_handle, nativeHandle))
	{
		DxLib::DeleteGraph(nativeHandle);

		cubeTextureStorage.Remove(_handle);
	}
}

// すべてのリソースを削除
void DxlibRenderer::DeleteAll()
{
	InitGraph();
}
