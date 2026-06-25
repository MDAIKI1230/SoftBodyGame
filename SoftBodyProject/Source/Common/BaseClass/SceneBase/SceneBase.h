#pragma once

#include <memory>

#include "SceneConstants.h"

#include "WorldStorage.h"
#include "SystemManager.h"

#include "ObjectManager.h"

#include "UpdateSystem.h"
#include "FixedUpdateSystem.h"
#include "RenderingSystem.h"
#include "SparseSetStorageBase.h"

class SceneBase
{
public:
	// ワールドストレージとシステムマネージャーを入れないと作れない。
	SceneBase(WorldStorage* _worldStorage, SystemManager* _systemManager);
	// 更新
	void Execute();
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
	// オブジェクトマネージャー取得
	ObjectManager* GetObjectManager() ;

	void FadeIn();
	void FadeOut();
	virtual void Initialize() = 0;
	virtual void Update();
	virtual void Terminate() = 0;
protected:
	// ワールドストレージ
	WorldStorage* worldStorage;
	// システムマネージャー
	SystemManager* systemManager;
	// シーンの状態
	SceneState state{ SceneState::INITIALIZE };
	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager;
};
