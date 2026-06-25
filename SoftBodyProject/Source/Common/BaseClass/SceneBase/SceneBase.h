#pragma once


#include <vector>
#include <memory>

#include "SceneConstants.h"

#include "WorldStorage.h"
#include "IWorld.h"

#include "ObjectManager.h"

#include "UpdateSystem.h"
#include "FixedUpdateSystem.h"
#include "RenderingSystem.h"
#include "SparseSetStorageBase.h"

class SceneBase:public IWorld
{
public:
	SceneBase();
	// 更新
	void Execute();
	// 描画
	void Draw();
	// 仮想デストラクタ
	virtual ~SceneBase() = default;
protected:
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<UpdateSystem> _system);
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<RenderingSystem> _system);
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="_system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<FixedUpdateSystem> _system);
	/// <summary>
	/// ストレージの追加(moveされる)
	/// </summary>
	/// <param name="storage">入れたいストレージ</param>
	template<typename T>
	void AddStorage(std::unique_ptr<SparseSetStorageBase<T>> _storage)
	{
		// コンテナに追加
		storages.push_back(std::move(_storage));
		// 対応付け
		storageMap[typeid(T)] = storages.size() - 1;
	}
	// オブジェクトマネージャー取得
	ObjectManager* GetObjectManager() override;

	void FadeIn();
	void FadeOut();
	virtual void Initialize() = 0;
	virtual void Update();
	virtual void Terminate() = 0;
protected:
	// ワールドストレージ
	WorldStorage* worldStorage;
	// シーンの状態
	SceneState state{ SceneState::INITIALIZE };
	// 更新系システム
	std::vector<std::unique_ptr<UpdateSystem>> updateSystems;
	// 物理ステップ更新系システム
	std::vector<std::unique_ptr<FixedUpdateSystem>> fixedUpdateSystems;
	// 描画系システム
	std::vector<std::unique_ptr<RenderingSystem>> renderingSystems;
	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager;
};
