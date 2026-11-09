#pragma once

#include <memory>

#include "MDJson.h"

#include "GizmoContext.h"

struct SceneDocument;
struct CollisionSettingDocument;

/*
	ComponentDocumentをまとめるための部分
	新しくComponentのDocumentを作りたい場合はComonentDocumentを継承すること。
*/
struct ComponentDocumentBase
{
public:
	// GetComponentのための識別
	using TypeID = const void*;

	template<class T>
	static TypeID TypeIDOf() noexcept
	{
		// 型ごとに別の変数が作られる。そのアドレスをキーにする
		static char key{};
		return &key;
	}

	ComponentDocumentBase(std::uint64_t _ownerObjectID, std::uint64_t _id, TypeID _typeID) :
		ownerObjectID{ _ownerObjectID },
		id{ _id },
		typeID{ _typeID }
	{
	}

	virtual std::unique_ptr<ComponentDocumentBase> Clone(std::uint64_t _ownerObjectI) = 0;

	virtual MDJson::Value SaveJson(MDJson::Allocator& _allocator) const = 0;

	// 見出しの選択クリックを戻り値、削除要求を引数で返す。
	virtual bool DrawInspector(const SceneDocument& _scene, const CollisionSettingDocument& _collisionSettings, bool& _removeRequested, bool _removable = true) = 0;

	// 所有Objectを参照して通常描画を行う。
	virtual void Draw(const ObjectDocument& _object) {};

	virtual void DrawGizmo(const GizmoContext& _context) {};

	virtual bool LoadJson(const MDJson::Value& _json) = 0;

	// Inspector・履歴・読み込みによる変更を通知する。
	virtual void OnChange() {};

	virtual void* FindFieldAddress(std::string_view _fieldName) = 0;

	virtual ~ComponentDocumentBase() = default;

	TypeID GetTypeID() const noexcept
	{
		return typeID;
	}

	std::uint64_t GetComponentID() const noexcept
	{
		return id;
	}

	// 複数追加可能かのチェック(基本trueなので、ダメな奴だけfalseに変える)
	virtual bool AllowsMultiple()
	{
		return true;
	}

protected:
	std::uint64_t id{ 0 };
	std::uint64_t ownerObjectID{ 0 };
private:
	TypeID typeID;
};