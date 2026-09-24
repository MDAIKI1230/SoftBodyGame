#pragma once

#include "SceneConstants.h"

class ApplicationRequest
{
public:
	static void ExitRequest()
	{
		isExitRequested = true;
	}
	static bool IsExitRequested()
	{
		return isExitRequested;
	}

	static void SceneRequest(SceneRequestType _sceneRequestType)
	{
		sceneRequestType = _sceneRequestType;
	}
	static SceneRequestType GetSceneRequest()
	{
		return sceneRequestType;
	}
private:
	inline static bool isExitRequested{ false };
	inline static SceneRequestType sceneRequestType{ SceneRequestType::NONE };
};