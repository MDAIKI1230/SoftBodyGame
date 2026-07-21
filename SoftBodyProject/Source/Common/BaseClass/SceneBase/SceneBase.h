#pragma once

#include <memory>
#include <string>

#include "SceneConstants.h"

#include "WorldStorage.h"
#include "SystemManager.h"
#include "EventManager.h"
#include "EventSystem.h"
#include "PhysicsWorld.h"

#include "ObjectManager.h"

#include "UpdateSystem.h"
#include "FixedUpdateSystem.h"
#include "RenderingSystem.h"
#include "SparseSetStorageBase.h"

class SceneBase
{
public:
	// ワールドストレージとシステムマネージャーを入れないと作れない。
	SceneBase();
	// 更新
	void Execute();
	void Render();
	// 仮想デストラクタ
	virtual ~SceneBase() = default;
protected:
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<UpdateSystem>&& _system);
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<RenderingSystem>&& _system);
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="_system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<FixedUpdateSystem>&& _system);
	/// <summary>
	/// ストレージの追加(moveされる)
	/// </summary>
	/// <param name="storage">入れたいストレージ</param>
	template<typename T>
	void AddStorage(std::unique_ptr<SparseSetStorageBase<T>>&& _storage)
	{
		worldStorage->AddStorage<T>(std::move(_storage));
	}

	void FadeIn();
	void FadeOut();
	virtual void Initialize() = 0;
	virtual void Update();
	virtual void Terminate() = 0;

	void LoadFile(std::string _filePath);
protected:
	std::unique_ptr<WorldStorage> worldStorage;
	std::unique_ptr<PhysicsWorld> physicsWorld;
	std::unique_ptr<SystemManager> systemManager;
	std::unique_ptr<EventManager> eventManager;
	std::unique_ptr<EventSystem> eventSystem;
	// シーンの状態
	SceneState state{ SceneState::INITIALIZE };
	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager;
};
