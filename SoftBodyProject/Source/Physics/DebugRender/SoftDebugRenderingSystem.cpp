#include "ServiceLocator.h"

#include "SoftDebugRenderingSystem.h"

void SoftDebugRenderingSystem::Initialize()
{
	handle = ServiceLocator::GetIGPUConnecter()->LoadVertexPixelShader("Shader/SoftDebugVS.vert", "Shader/SoftDebugFS.frag");
}

void SoftDebugRenderingSystem::Render()
{
	ServiceLocator::GetIGPUConnecter()->BeginGraphicsShader(handle);
	
	ServiceLocator::GetRenderer()->DrawPoint();

	ServiceLocator::GetIGPUConnecter()->EndGraphicsShader();
}
