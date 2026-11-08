#pragma once

#include <memory>
#include <string>

#include "SceneConstants.h"
#include "SceneFileData.h"

#include "WorldStorage.h"
#include "SystemManager.h"
#include "EventManager.h"
#include "EventSystem.h"
#include "PhysicsWorld.h"
#include "AnimationWorld.h"

#include "ObjectManager.h"

#include "LoadingScreen/LoadingScreen.h"
#include "ResourceOwnerHandle.h"

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
	void End(std::unique_ptr<LoadingScreen>&& _loadingScreen);
	// 切り替えていいよフラグ
	bool CompleteEnding();
	// ロードスクリーン変更
	void SetLoadScreen(std::unique_ptr<LoadingScreen>&& _loadingScreen) { loadingScreen = std::move(_loadingScreen); }
	// ロードスクリーンを渡す
	std::unique_ptr<LoadingScreen>&& MoveLoadScreen() { return std::move(loadingScreen); }
	// 仮想デストラクタ
	virtual ~SceneBase() = default;
protected:
	virtual void Initialize() = 0;

	void LoadFile(std::string _filePath);

	template<class Object>
	Object* Create();
private:
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

	void StartLoading();
	void EndLoading();
	void Update();
	void Terminate();

	void CreateObjects(std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneFileData& _data);
	void CreateHierarchy(const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneFileData& _data);
	void SettingTransform(
		const std::unordered_map<std::uint64_t, EntityID>& _idMap,
		std::unordered_map<std::uint64_t, bool>& _settingMap,
		const SceneFileData& _data, const SceneObjectData& _objectData);
	void CreateComponents(const std::unordered_map<std::uint64_t, EntityID>& _idMap, const SceneFileData& _data);
private:
	ResourceOwnerHandle resourceOwnerID;
	WorldStorage worldStorage;
	PhysicsWorld physicsWorld;
	AnimationWorld animationWorld;
	SystemManager systemManager;
	EventManager eventManager;
	EventSystem eventSystem;
	// シーンの状態
	SceneState state{ SceneState::START_LOADING };
	// オブジェクトマネージャー
	ObjectManager objectManager;

	//シーン切り替えていいよフラグ
	bool isCompleteEnding{ false };

	// ロード中クラス
	std::unique_ptr<LoadingScreen> loadingScreen{ nullptr };
};

#include "SceneBase.inl"