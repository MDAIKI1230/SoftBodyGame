#pragma once

#include <cstdint>

#include "FieldMacros.h"

#include "Base/ComponentDocument.h"

#include "MDMath.h"

struct SceneDocument;

struct TransformComponentDocument :public ComponentDocument<TransformComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	Matrix4x4 GetRotationMatrix() const;
	Matrix4x4 GetMatrix() const;

	std::uint64_t GetParentObjectID() const;
	void SetParentObjectID(std::uint64_t _parentObjectID);

	bool IsDirty() const;
	void ClearDirty();

	// ローカル値か親が変わった場合にワールド行列を再計算する。キャッシュのみ更新する。
	bool RecalculateMatrix(const Matrix4x4& _parentMatrix, std::uint64_t _resolvedParentID, bool _parentChanged) const;

	void DrawGizmo(const GizmoContext& _context) override;
	void OnChange() override;
	bool LoadJson(const MDJson::Value& _json) override;

public:
	Vector3 position;
	Vector3 rotationDegrees;
	Vector3 scale{ Vector3::ONE };

	MD_DOCUMENT(
		TransformComponentDocument,
		"Transform",

		MD_FIELD("ParentObjectID", parentObjectID,
			MD_GUI_HIDDEN()),

		MD_FIELD("Position", position),

		MD_FIELD("RotationDeg", rotationDegrees,
			MD_GUI_LABEL("Rotation"),
			MD_GUI_UNIT("deg")),

		MD_FIELD("Scale", scale,
			MD_GUI_SPEED(0.01f))
	);

private:
	void SyncTransform() const;
	bool GetParentSpace(SceneDocument& _scene, Matrix4x4& _worldToLocal, Quaternion& _parentRotation) const;
private:
	// 0は親なし。
	std::uint64_t parentObjectID{ 0 };
	// 内部TransformはローカルTRSを保持し、親はObject IDでのみ管理する。
	mutable Transform transform;
	mutable Matrix4x4 parentMatrix{ Matrix4x4::IDENTITY };
	mutable Matrix4x4 worldMatrix{ Matrix4x4::IDENTITY };
	mutable std::uint64_t appliedParentObjectID{ 0 };
	mutable Vector3 appliedRotationDegrees;
	// 階層の更新が完了した側で解除する。
	mutable bool isDirty{ true };
};