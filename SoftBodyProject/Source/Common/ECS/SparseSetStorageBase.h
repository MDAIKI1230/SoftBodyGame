#pragma once

#include "StorageBase.h"
#include "SparseSet.h"

template<typename T>
class SparseSetStorageBase :public StorageBase
{
public:
	// コンストラクタ
	SparseSetStorageBase() = default;
	/// <summary>
	/// 取得
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <returns></returns>
	T* Get(int _entity) { return sparseSet.Get(_entity); }
	/// <summary>
	/// 追加
	/// </summary>
	/// <param name="entity">エンティティID</param>
	/// <param name="component">追加コンポーネント</param>
	void Add(int _entity, const T& _obj) { sparseSet.Add(_entity, _obj); }
	/// <summary>
	/// 除外
	/// </summary>
	/// <param name="entity">エンティティID</param>
	void Remove(int _entity) { sparseSet.Remove(_entity); }
	// サイズ生成
	void Reserve(size_t _size) { sparseSet.Reserve(_size); }
	// 全削除
	void Clear() { sparseSet.Clear(); }
	/// <summary>
	/// 取得できるか
	/// </summary>
	/// <param name="output">取得したコンポーネント</param>
	/// <returns>取得できたか</returns>
	bool TryGet(int _entity, T& _output) { return sparseSet.TryGet(_entity, _output); }
	/// <summary>
	/// 持っているか
	/// </summary>
	/// <param name="target">対象</param>
	/// <returns>持っているか</returns>
	bool Has(int _entity) { return sparseSet.Has(_entity); }
	// サイズ
	size_t GetSize() { return sparseSet.GetSize(); }
	// 実データコンテナ取得
	std::vector<T>* GetDense() { return sparseSet.GetDense(); }
	// エンティティコンテナ取得
	std::vector<int>* GetEntities() { return sparseSet.GetEntities(); }

	// 仮想デストラクタ
	virtual ~SparseSetStorageBase() = default;
protected:
	SparseSet<T> sparseSet{};
};