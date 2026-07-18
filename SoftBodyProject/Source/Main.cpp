#include <Windows.h>
#include <memory>
#include "ApplicationManager.h"

#include "BackEndFactory.h"


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::unique_ptr<ApplicationManager> applicationManager = std::make_unique<ApplicationManager>(BackEndFactory::CreateBackEnd());

	return applicationManager->ApplicationMain();				// ソフトの終了
}
