# include "SceneFactory.h"

#include "AssertMacros.h"

#include "SceneManager.h"

// 初期化用関数
template<FixedString Name>
std::unique_ptr<SceneBase> SceneManager::CreateInitialScene()
{
	if constexpr (requires { SceneFactory::Create<Name>(); })
	{
		return SceneFactory::Create<Name>();
	}
	else
	{
		MD_DEBUG_LOG_IF(Name.value[0] == '\0', "初期Sceneが登録されていません。");

		MD_UNREACHABLE("生成出来ないシーンが設定されています。");
	}
}