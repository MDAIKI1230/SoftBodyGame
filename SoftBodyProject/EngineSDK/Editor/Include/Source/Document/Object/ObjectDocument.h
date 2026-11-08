#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <vector>

#include "../Component/TransformComponentDocument.h"

#include "MDMath.h"
#include "PropertySnapshot.h"

struct SceneDocument;

struct ObjectDocument
{
	friend class SceneSerializer;
public:
	ObjectDocument(std::uint64_t _id);
	ObjectDocument(std::uint64_t _id, std::string _type);

	// 固定Transformは読み取り専用で公開する。
	const TransformComponentDocument& GetTransform() const;
	Matrix4x4 GetMatrix() const;
	// ローカル行列設定
	void SetLocalMatrix(const Matrix4x4 _mat);

	// Transformを先頭に描画し、削除要求のComponent IDを返す。
	std::uint64_t DrawInspector(const SceneDocument& _scene, std::uint64_t& _selectedComponentID);
	void Draw();
	void DrawGizmo(const GizmoContext& _context);

	// 見つからなければnullptr
	template<class T>
	T* GetComponent();

	template<class T>
	const T* GetComponent() const;

	const ComponentDocumentBase* FindComponent(std::uint64_t _id) const;
	// 同じComponentを持ってるか判定
	bool HasComponent(const ComponentDocumentBase* _component) const;

	// 作成してcomponentsへ追加し、追加した実体を返す
	template<class T, class... Args>
	T* AddComponent(Args&&... _args);

	ComponentDocumentBase* AddComponent(const std::string& _name);

	// 指定したComponent IDの要素を削除する。固定Transformは削除しない。
	bool RemoveComponent(std::uint64_t _id);

	ObjectDocument Clone(std::uint64_t _id) const;

	// 履歴の適用は内部で行い、変更可能なフィールドのアドレスを外へ出さない。
	bool HasProperty(const PropertyTarget& _target);
	bool ApplyProperty(const PropertyTarget& _target, const PropertySnapshot& _snapshot);

	// 選択されたオブジェクトかチェックする関数
	bool IsSelected(std::uint64_t _selectedID);

	// ID取得
	std::uint64_t GetId() const { return id; }
	// 追加・削除できるコンポーネントだけを走査する。
	std::span<const std::unique_ptr<ComponentDocumentBase>> GetComponents() const { return components; }

	// 無効値チェック
	bool IsValid() { return id != 0; }
public:
	std::string name{ "Test Object" };
private:
	void* ResolveProperty(const PropertyTarget& _target);
	ComponentDocumentBase* FindEditableComponent(std::uint64_t _id);
private:
	static constexpr std::uint64_t transformComponentID{ 1 };
	std::string type{ "EmptyObject" };

	std::vector<std::unique_ptr<ComponentDocumentBase>> components;
	// 0は未設定として扱う
	std::uint64_t id{ 0 };
	std::uint64_t nextComponentID{ transformComponentID + 1 };
	TransformComponentDocument transform;
};

#include "ObjectDocument.inl"