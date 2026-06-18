#pragma once

#include <utility>

#include "IWorld.h"

class ObjectBase
{
public:
	// コンストラクタ
	ObjectBase(IWorld* _world, int _handle) :
		world{ _world },
		handle{ _handle }
	{
	}

	// --- 更新系 ---

	// 更新処理
	virtual void Update() = 0;
	// 物理更新処理
	virtual void FixedUpdate() = 0;

	// --- 衝突系 ---
	
	// 衝突始め
	virtual void OnCollisionEnter() = 0;
	// 衝突中ずっと
	virtual void OnCollision() = 0;
	// 衝突終わり
	virtual void OnCollisionExit() = 0;

	// --- コンポーネント ---

	// 追加
	template<typename T,typename ... Args>
	T* AddComponent(Args&&... args)
	{
		SparseSetStorageBase<T>* storage{ world->GetStorage<T>() };

		if(storage != nullptr)
		{
			return storage->Add(handle, T{ std::forward<Args>(args)... });
		}

		return nullptr;
	}

	// 取得
	template<typename T>
	T* GetComponent()
	{
		SparseSetStorageBase<T>* storage{ world->GetStorage<T>() };

		if (storage != nullptr)
		{
			return storage->Get(handle);
		}

		return nullptr;
	}

	// --- ゲッター　---
	int GetHandle() { return handle; }
	// 仮想デストラクタ
	virtual ~ObjectBase() = default;
private:
	int handle;
	IWorld* world;
};