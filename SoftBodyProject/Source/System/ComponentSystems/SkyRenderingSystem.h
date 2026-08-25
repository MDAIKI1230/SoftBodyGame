#pragma once

#include "RenderingSystem.h"

#include "ModelHandle.h"
#include "VertexShaderHandle.h"
#include "PixelShaderHandle.h"

class SkyRenderingSystem :public RenderingSystem
{
public:
	// 初期化
	void Initialize() override;
	// 描画
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
private:
	struct SkySolidConstantBuffer
	{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Color skyColor;
		uint32_t solidFlag;
	};

	ModelHandle sphereHandle;
	ShaderConstantBufferHandle constantBufferHandle;
	VertexShaderHandle solidSkyVertexShader;
	PixelShaderHandle solidSkyPixelShader;
};
