#pragma once

#include <cstddef>
#include <string_view>

#include "PropertyTarget.h"

#include "ComponentDocumentBase.h"

/*
	実際の、各種コンポーネントドキュメントに継承する構造体
	共通で、欲しい関数をテンプレートでもらってから実装する
	テンプレートには、各種コンポーネントをそのまま渡す
*/
template<class Derived>
struct ComponentDocument:public ComponentDocumentBase
{
	ComponentDocument(std::uint64_t _ownerObjectID, std::uint64_t _id) :
		ComponentDocumentBase{ _ownerObjectID,_id, ComponentDocumentBase::TypeIDOf<Derived>() }
	{
	}

	std::unique_ptr<ComponentDocumentBase> Clone(std::uint64_t _ownerObjectI) override;

	MDJson::Value SaveJson(MDJson::Allocator& _allocator) const override;

	bool LoadJson(const MDJson::Value& _json) override;

	bool DrawInspector(const SceneDocument& _scene, const CollisionSettingDocument& _collisionSettings, bool& _removeRequested, bool _removable = true) override;

	void* FindFieldAddress(std::string_view _fieldName) override;
private:
	// Fieldを順番に描画し、グループの切り替え時にテーブルを区切る。
	template<std::size_t Index>
	void DrawInspectorFields(const SceneDocument& _scene, const CollisionSettingDocument& _collisionSettings, const PropertyTarget& _target, std::string_view& _groupName, bool& _groupOpen, bool& _tableOpen, bool& _changed);
};

#include "ComponentDocument.inl"