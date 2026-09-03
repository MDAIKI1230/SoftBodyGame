#include "BackEndFactory.h"

// DXライブラリ
#include "Dxlib\DxlibRenderer.h"
#include "Dxlib\DxlibInput.h"
#include "Dxlib\DxlibSystem.h"
#include "Dxlib\DxlibGPUConnecter.h"

BackEnd BackEndFactory::CreateBackEnd()
{
	BackEnd backend;

	backend.renderer = std::make_unique<DxlibRenderer>();
	backend.inputSystem = std::make_unique<InputSystem>(std::make_unique<DxlibInput>());
	backend.system = std::make_unique<DxlibSystem>();
	backend.gpuConnecter = std::make_unique<DxlibGPUConnecter>();

	return backend;
}
