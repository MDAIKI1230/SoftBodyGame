#include <Windows.h>
#include <memory>
#include "System/ApplicationManager.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::unique_ptr<ApplicationManager> applicationManager = std::make_unique<ApplicationManager>();

	return applicationManager->ApplicationMain();				// ソフトの終了
}