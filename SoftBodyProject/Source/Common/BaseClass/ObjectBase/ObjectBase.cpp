#include "IWorld.h"

#include "ObjectBase.h"

template<typename T, typename ... Args>
void ObjectBase::AddComponent(Args&&... args)
{
	// 作成
	T component{ args };
	// ストレージ取得
	SparseSetStorageBase<T>* storage{ handle.GetWorldPtr()->GetStorage<T>() };

	// nullチェック
	if (storage != nullptr)
	{
		// 追加
		storage->Add(handle, component);
	}
}