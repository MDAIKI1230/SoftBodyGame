#pragma once

#include "SingleTonMacros.h"

class GameManager
{
public:
	static void StartScene()
	SingletonPublicFunction(void, StartScene, (), ())
	static void StartEscape()
	SingletonPublicFunction(void, StartEscape, (), ())

	static bool IsScene()
	SingletonPublicFunction(bool, IsScene, (), ())
	static bool IsEscape()
	SingletonPublicFunction(bool, IsEscape, (), ())
private:
	// シングルトン
	// コンストラクタ
	GameManager() = default;
	static GameManager& Instance()
	{
		static GameManager Instance;
		return Instance;
	}
private:
	enum class CurrentPhase
	{
		SCENE,
		ESCAPE
	};

	CurrentPhase phase{ CurrentPhase::SCENE };
};