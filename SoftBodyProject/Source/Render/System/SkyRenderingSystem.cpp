#include "BaseConstants.h"
#include "ServiceLocator.h"

#include "TransformComponentStorage.h"
#include "CameraComponentStorage.h"

#include "SkyRenderingSystem.h"

void SkyRenderingSystem::Initialize()
{
	ServiceLocator::GetResourceManager()->LoadModel("Res/Model/SkyCube/SkyCube.mv1");
	boxHandle = ServiceLocator::GetResourceManager()->GetModel("SkyCube.mv1");
	ServiceLocator::GetResourceManager()->LoadVertexShader("Shader/HLSL/Sky/SkyVS.vso");
	ServiceLocator::GetResourceManager()->LoadPixelShader("Shader/HLSL/Sky/SkyPS.pso");
	solidSkyVertexShader = ServiceLocator::GetResourceManager()->GetVertexShader("SkyVS.vso");
	solidSkyPixelShader = ServiceLocator::GetResourceManager()->GetPixelShader("SkyPS.pso");

	constantBufferHandle = ServiceLocator::GetGPUConnecter()->CreateConstantBuffer(sizeof(SkySolidConstantBuffer));
}

// 描画
void SkyRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };
	// Cameraストレージ
	CameraComponentStorage* cameraStorage{ static_cast<CameraComponentStorage*>(_worldStorage->GetStorage<CameraComponent>()) };

	ServiceLocator::GetGPUConnecter()->BeginVertexShader(solidSkyVertexShader);
	ServiceLocator::GetGPUConnecter()->BeginPixelShader(solidSkyPixelShader);

	for (auto cameraEntity : cameraStorage->GetEntities())
	{
		const TransformComponent& trans{ transformStorage->Get(cameraEntity) };

		SkySolidConstantBuffer cbData;

		const CameraComponent& camera{ cameraStorage->Get(cameraEntity) };

		// モデルをカメラの位置に持ってくる。
		ServiceLocator::GetRenderer()->ModelSetMatrix(boxHandle, trans.GetWorldMatrix());

		cbData.world = Matrix4x4::Identity();
		cbData.view = MatGenerateFunc::InverseTRS(trans.GetPosition(), trans.GetRotation(), Vector3{ 1.0f,1.0f, 1.0f });

		// Projection行列作成
		const ViewPort& vp{ camera.GetViewPort() };

		float drawWidth{Config::WINDOW_SIZE_W * vp.width};

		float drawHeight{ Config::WINDOW_SIZE_H * vp.height};

		float aspect{drawWidth / drawHeight};

		cbData.projection = MatGenerateFunc::Projection(camera.GetFov() * MathConstants::PI_FLT / 180.0f, aspect, camera.GetNear(), camera.GetFar());

		cbData.skyColor = camera.GetSolidColor();

		cbData.solidFlag = static_cast<uint32_t>(camera.GetClearMode());

		ServiceLocator::GetRenderer()->BindCubeTexture(camera.GetSkyTextureHandle(), 0);

		// 定数バッファに値渡して上げる
		void* pBuffer{ ServiceLocator::GetGPUConnecter()->GetConstantBufferAddress(constantBufferHandle) };

		memcpy(pBuffer, &cbData, sizeof(SkySolidConstantBuffer));

		ServiceLocator::GetGPUConnecter()->UpdateConstantBuffer(constantBufferHandle, &cbData, sizeof(SkySolidConstantBuffer));
		
		ServiceLocator::GetGPUConnecter()->BindConstantBufferPixel(constantBufferHandle, 4);
		ServiceLocator::GetGPUConnecter()->BindConstantBufferVertex(constantBufferHandle, 4);

		ServiceLocator::GetRenderer()->DrawModel(boxHandle);
	}

	ServiceLocator::GetGPUConnecter()->EndGraphicsShader();
}
