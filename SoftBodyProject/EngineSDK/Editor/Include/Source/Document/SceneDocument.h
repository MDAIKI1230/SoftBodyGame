#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <string>

#include "Object/ObjectDocument.h"

#include "PropertyTarget.h"
#include "PropertySnapshot.h"

struct SceneDocument
{
	friend class SceneSerializer;
public:
	// IDによってオブジェクトを返す関数
	ObjectDocument* FindObject(std::uint64_t _id);
	// 選択中のオブジェクト取得(ないならnullptr)
	ObjectDocument* GetSelectedObject();
	// オブジェクトを新たに作成
	std::uint64_t CreateObject(const std::string& _name = "New Object");
	// オブジェクトの複製
	std::uint64_t DuplicateObject(std::uint64_t _id);
	// 指定Objectとすべての子孫を削除
	bool DeleteObject(std::uint64_t _id);
	// 親IDの存在と、自分自身・子孫を親にしないことを確認する。0は親なし。
	bool CanSetObjectParent(std::uint64_t _objectID, std::uint64_t _parentObjectID);
	// 親変更を検証して適用する。値が変わった場合にtrueを返す。
	bool SetObjectParent(std::uint64_t _objectID, std::uint64_t _parentObjectID);
	// シーンの編集データを初期化する
	void Clear();
	// 編集対象の確認と、履歴の値の適用。
	bool HasProperty(const PropertyTarget& _target);
	bool ApplyProperty(const PropertyTarget& _target, const PropertySnapshot& _snapshot);
	// 親子関係の計算
	void UpdateTransforms();
public:
	std::vector<ObjectDocument> objects;
	// 0は選択されてない状態
	std::uint64_t selectedObjectID{ 0 };
	std::uint64_t selectedComponentID{ 0 };
private:
	// 新たなオブジェクトID取得
	std::uint64_t AllocateObjectID();

	// 走査中の循環検出と、親から子への再計算結果の伝播に使う。
	enum class TransformUpdateState
	{
		Unvisited,
		Visiting,
		Unchanged,
		Changed,
		Failed
	};

	TransformUpdateState UpdateTransform(std::size_t _index, const std::unordered_map<std::uint64_t, std::size_t>& _indices, std::vector<TransformUpdateState>& _states);
private:
	std::uint64_t nextObjectID{ 1 };
};