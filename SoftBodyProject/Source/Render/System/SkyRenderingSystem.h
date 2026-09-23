#pragma once

#include "MDMath.h"

#include "RenderingSystem.h"

#include "ModelHandle.h"
#include "ShaderConstantBufferHandle.h"
#include "PixelShaderHandle.h"
#include "VertexShaderHandle.h"

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
		uint32_t solidFlag{ 0 };
	};

	ModelHandle boxHandle;
	ShaderConstantBufferHandle cbHandle;
	VertexShaderHandle vertex;
	PixelShaderHandle pixel;
};
