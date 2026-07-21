#include "BackEndFactory.h"

#ifdef  USE_RAYLIB

// レイライブラリ
#include "Raylib\RaylibRenderer.h"
#include "Raylib\RaylibInput.h"
#include "Raylib\RaylibSystem.h"
#include "Raylib\RaylibGPUConnecter.h"

#else

// DXライブラリ
#include "Dxlib\DxlibRenderer.h"
#include "Dxlib\DxlibInput.h"
#include "Dxlib\DxlibSystem.h"

#endif //  USE_RAYLIB

BackEnd BackEndFactory::CreateBackEnd()
{
	BackEnd backend;

#ifdef USE_RAYLIB
	backend.renderer = std::make_unique<RaylibRenderer>();
	backend.input = std::make_unique<RaylibInput>();
	backend.system = std::make_unique<RaylibSystem>();
	backend.gpuConnecter = std::make_unique<RaylibGPUConnecter>();
#else
	backend.renderer = std::make_unique<DxlibRenderer>();
	backend.input = std::make_unique<DxlibInput>();
	backend.system = std::make_unique<DxlibSystem>();
#endif //  USE_RAYLIB

	return backend;
}
