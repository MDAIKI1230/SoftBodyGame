#pragma once

#include <optional>

#include "FieldMacros.h"
#include "LightConstants.h"
#include "LightHandle.h"
#include "MDMath.h"

#include "Base/ComponentDocument.h"

#include "Color.h"

struct LightComponentDocument : public ComponentDocument<LightComponentDocument>
{
public:
	LightComponentDocument(std::uint64_t _ownerObjectID, std::uint64_t _id);
	~LightComponentDocument() override;

	void OnChange() override;

	void DrawGizmo(const GizmoContext& _context) override;
	bool AllowsMultiple() override;

public:
	LightType type{ LightType::DIRECTIONAL };
	Vector3 color{ 1.0f, 1.0f, 1.0f };
	bool enabled{ true };
	float intensity{ 1.0f };
	float range{ 0.0f };
	float innerAngleDegrees{ 0.0f };
	float outerAngleDegrees{ 0.0f };

	MD_DOCUMENT(
		LightComponentDocument,
		"Light",

		MD_FIELD("Type", type,
			MD_GUI_ENUM_ITEMS(
				MD_GUI_ENUM_ITEM("Directional", LightType::DIRECTIONAL),
				MD_GUI_ENUM_ITEM("Point", LightType::POINT),
				MD_GUI_ENUM_ITEM("Spot", LightType::SPOT))),

		MD_FIELD("Color", color, MD_GUI_COLOR()),
		MD_FIELD("Enabled", enabled),

		MD_FIELD("Intensity", intensity,
			MD_GUI_MIN(0.0f),
			MD_GUI_CLAMP()),

		MD_FIELD("Range", range,
			MD_GUI_MIN(0.0f),
			MD_GUI_CLAMP()),

		MD_FIELD("InnerAngleDeg", innerAngleDegrees,
			MD_GUI_LABEL("Inner Angle"),
			MD_GUI_RANGE(0.0f, 180.0f),
			MD_GUI_CLAMP(),
			MD_GUI_UNIT("deg")),

		MD_FIELD("OuterAngleDeg", outerAngleDegrees,
			MD_GUI_LABEL("Outer Angle"),
			MD_GUI_RANGE(0.0f, 180.0f),
			MD_GUI_CLAMP(),
			MD_GUI_UNIT("deg"))
	);
private:
	friend class ScenePanel;
	friend bool MDJson::LoadDocument<LightComponentDocument>(const MDJson::Value& _json, LightComponentDocument& _document);

	LightComponentDocument(const LightComponentDocument& _other);
	LightComponentDocument& operator=(const LightComponentDocument& _other);

	bool BeginPreview(const ObjectDocument& _object, SceneDocument& _scene);
	void EndPreview();
	static Matrix4x4 GetWorldRotation(const ObjectDocument& _object, SceneDocument* _scene);
	static void DrawArrow(const Vector3& _origin, const Vector3& _forward, const Vector3& _right,
		float _length, const Color& _color);
	static void DrawSpotCone(const Vector3& _origin, const Vector3& _forward, const Vector3& _right,
		float _range, float _angleDegrees, const Color& _color);

private:
	LightHandle light;
	std::optional<Vector3> lastPosition;
	std::optional<Vector3> lastDirection;
};