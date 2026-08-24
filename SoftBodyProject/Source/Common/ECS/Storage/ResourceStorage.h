#pragma once

#include "SparseSet.h"
#include "HandlePool.h"
#include "LinearTable.h"

template<class HANDLE,class DATA>
class ResourceStorage
{
public:
	// 持ってるか確認
	bool Has(HANDLE _handle)
	{
		if (!IsAlive(_handle))
		{
			return false;
		}

		return resources.Has(_handle);
	}
	// ハンドルが生きているか
	bool IsAlive(HANDLE _handle)
	{
		return handlePool.IsAlive(_handle);
	}
	// 取得
	bool TryGet(HANDLE _handle, DATA& _output)
	{
		if (!handlePool.IsAlive(_handle))
		{
			return false;
		}

		DATA* data = resources.TryEdit(_handle);
		if (data == nullptr)
		{
			return false;
		}

		_output = *data;
		return true;
	}
	// 破棄
	void Remove(HANDLE _handle)
	{
		if (!handlePool.IsAlive(_handle))
			return;

		resources.Remove(_handle);
		handlePool.Release(_handle);
	}
	// 追加
	HANDLE Add(DATA _data)
	{
		// ハンドルのIndexを決定
		HANDLE handle{ handlePool.Acquire() };
		resources.Add(handle, std::move(_data));
		return handle;
	}
	/// <summary>
	/// 全削除
	/// </summary>
	/// <param name="_destroy">破棄関数を入れてね</param>
	template<class DESTROY_FUNC>
	void Clear(DESTROY_FUNC&& _destroy)
	{
		// ネイティブリソース破棄
		for (const auto& resource : resources.GetDenseRange())
		{
			_destroy(resource);
		}

		// Denseデータ削除
		resources.Clear();

		// 全Handleを無効化
		handlePool.Clear();
	}
private:
	HandlePool<HANDLE> handlePool;
	SparseSet<DATA, HANDLE, LinearTable> resources;
};
