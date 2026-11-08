#pragma once

#include "SceneBase.h"

#include "SceneFactory.h"

class ApplicationRequest
{
	using SceneCreateFunc = std::unique_ptr<SceneBase>(*)();
public:
	static void ExitRequest()
	{
		isExitRequested = true;
	}
	static bool IsExitRequested()
	{
		return isExitRequested;
	}

	template<FixedString Name>
	static void SceneRequest(std::unique_ptr<LoadingScreen>&& _loadingScreen = 0)
	{
		sceneCreteFunc = &SceneFactory::Create<Name>;
		loadingScreen = std::move(_loadingScreen);
	}

	static bool NeedChange()
	{
		return sceneCreteFunc;
	}

	static std::unique_ptr<SceneBase> ApplySceneRequest()
	{
		std::unique_ptr<SceneBase> resutl{ sceneCreteFunc() };
		sceneCreteFunc = nullptr;
		return std::move(resutl);
	}

	static std::unique_ptr<LoadingScreen> GetRequestLoadingScreen()
	{
		return std::move(loadingScreen);
	}
private:
	inline static bool isExitRequested{ false };
	inline static SceneCreateFunc sceneCreteFunc{ nullptr };
	inline static std::unique_ptr<LoadingScreen> loadingScreen;
};