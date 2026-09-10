#pragma once

#include <memory>
#include <string>

#include "SceneConstants.h"

#include "WorldStorage.h"
#include "SystemManager.h"
#include "EventManager.h"
#include "EventSystem.h"
#include "PhysicsWorld.h"
#include "AnimationWorld.h"

#include "ObjectManager.h"

#include "UpdateSystem.h"
#include "FixedUpdateSystem.h"
#include "RenderingSystem.h"

class SceneBase
{
public:
	// ワールドストレージとシステムマネージャーを入れないと作れない。
	SceneBase();
	// 更新
	void Execute();
	// 描画
	void Render();
	// 終了
	void End();
	// 切り替えていいよフラグ
	bool CompleteEnding();
	// 仮想デストラクタ
	virtual ~SceneBase();
protected:
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	template<class T>
	void AddSystem(T&& _system)
	{
		systemManager.AddSystem(std::move(_system));
	}
	/// <summary>
	/// ストレージの追加(moveされる)
	/// </summary>
	/// <param name="storage">入れたいストレージ</param>
	template<typename T>
	void AddStorage(std::unique_ptr<StorageBase>&& _storage)
	{
		worldStorage.AddStorage<T>(std::move(_storage));
	}

	void FadeIn();
	void FadeOut();
	virtual void Initialize() = 0;
	virtual void Update();
	void Terminate();

	void LoadFile(std::string _filePath);
protected:
	WorldStorage worldStorage;
	PhysicsWorld physicsWorld;
	AnimationWorld animationWorld;
	SystemManager systemManager;
	EventManager eventManager;
	EventSystem eventSystem;
	// シーンの状態
	SceneState state{ SceneState::INITIALIZE };
	// オブジェクトマネージャー
	ObjectManager objectManager;

	//シーン切り替えていいよフラグ
	bool isCompleteEnding{ false };
};
