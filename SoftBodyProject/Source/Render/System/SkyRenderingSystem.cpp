#include "BaseConstants.h"

#include "ResourceManager.h"
#include "Renderer.h"

#include "TransformComponentStorage.h"
#include "CameraComponentStorage.h"

#include "SkyRenderingSystem.h"

void SkyRenderingSystem::Initialize()
{
	ResourceManager::LoadModel("Res/Model/SkyCube/SkyCube.mv1");
	boxHandle = ResourceManager::GetModel("SkyCube.mv1");
	ResourceManager::LoadVertexShader("Shader/Sky/SkyVS.vso");
	ResourceManager::LoadPixelShader("Shader/Sky/SkyPS.pso");
	vertex = ResourceManager::GetVertexShader("SkyVS.vso");
	pixel = ResourceManager::GetPixelShader("SkyPS.pso");

	cbHandle = ResourceManager::CreateConstantBuffer(sizeof(SkySolidConstantBuffer));
}

// 描画
void SkyRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };
	// Cameraストレージ
	CameraComponentStorage* cameraStorage{ static_cast<CameraComponentStorage*>(_worldStorage->GetStorage<CameraComponent>()) };

	for (auto cameraEntity : cameraStorage->GetEntities())
	{
		Material material;

		material.SetVertex(vertex);
		material.SetPixel(pixel);
		material.SetConstantBuffer(cbHandle, 4);

		const TransformComponent& trans{ transformStorage->Get(cameraEntity) };

		SkySolidConstantBuffer cbData;

		const CameraComponent& camera{ cameraStorage->Get(cameraEntity) };

		// モデルをカメラの位置に持ってくる。
		ResourceManager::SetMatrix(boxHandle, trans.GetWorldMatrix());

		cbData.world = Matrix4x4::IDENTITY;
		cbData.view = MatGenerateFunc::InverseTRS(trans.GetPosition(), trans.GetRotation(), Vector3{ 1.0f,1.0f, 1.0f });

		// Projection行列作成
		const ViewPort& vp{ camera.GetViewPort() };

		float drawWidth{Config::WINDOW_SIZE_W * vp.width};

		float drawHeight{ Config::WINDOW_SIZE_H * vp.height};

		float aspect{drawWidth / drawHeight};

		cbData.projection = MatGenerateFunc::Projection(camera.GetFov() * MathConstants::PI_FLT / 180.0f, aspect, camera.GetNear(), camera.GetFar());

		cbData.skyColor = camera.GetSolidColor();

		cbData.solidFlag = static_cast<uint32_t>(camera.GetClearMode());

		material.SetCubeTexture(camera.GetSkyTextureHandle(), 0);

		// 定数バッファに値渡して上げる
		ResourceManager::SetConstantBufferValue<SkySolidConstantBuffer>(cbHandle, &cbData);

		Renderer::DrawModel(boxHandle, material);
	}
}
