#pragma once

#include <memory>

#include "FixedString.h"

#include "SceneBase.h"

class SceneFactory
{
public:
	// ビルド時の設定から初期Sceneを作成する。
	static std::unique_ptr<SceneBase> CreateInitialScene();

	template<FixedString Name>
	static std::unique_ptr<SceneBase> Create() = delete;
};

#include "SceneFactory.inl"