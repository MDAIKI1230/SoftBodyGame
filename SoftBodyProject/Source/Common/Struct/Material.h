#pragma once

#include <vector>

#include "VertexShaderHandle.h"
#include "PixelShaderHandle.h"
#include "ShaderConstantBufferHandle.h"
#include "TextureHandle.h"
#include "CubeTextureHandle.h"

/*
	今は、シェーダをつめてRendererに渡す用
	今後、拡張していく
*/
struct Material
{
	friend class Renderer;
public:
	// 頂点シェーダセット
	void SetVertex(VertexShaderHandle _vertex) { vertex = _vertex; }
	// ピクセルシェーダセット
	void SetPixel(PixelShaderHandle _pixel) { pixel = _pixel; }

	// 定数バッファセット
	void SetConstantBuffer(ShaderConstantBufferHandle _cbHandle, uint32_t _slot)
	{
		cibstantBuffers.push_back(_cbHandle);
		cbBindSlotNums.push_back(_slot);
	}

	// 画像バッファセット
	void SetTexture(TextureHandle _textureHandle, uint32_t _slot)
	{
		bindTextures.push_back(_textureHandle);
		texBindSlotNums.push_back(_slot);
	}

	// キューブテクスチャバッファセット
	void SetCubeTexture(CubeTextureHandle _cbHandle, uint32_t _slot)
	{
		bindCubeTextures.push_back(_cbHandle);
		cubeTexBindSlotNums.push_back(_slot);
	}

private:
	// 頂点
	VertexShaderHandle vertex;
	// ピクセル
	PixelShaderHandle pixel;

	// 定数バッファ
	std::vector<ShaderConstantBufferHandle> cibstantBuffers;
	// 定数バッファのバインドスロット番号
	std::vector<uint32_t> cbBindSlotNums;

	// 画像
	std::vector<TextureHandle> bindTextures;
	// 画像のバインドスロット番号
	std::vector<uint32_t> texBindSlotNums;

	// キューブテクスチャ
	std::vector<CubeTextureHandle> bindCubeTextures;
	// キューブテクスチャバインドスロット番号
	std::vector<uint32_t> cubeTexBindSlotNums;
};
