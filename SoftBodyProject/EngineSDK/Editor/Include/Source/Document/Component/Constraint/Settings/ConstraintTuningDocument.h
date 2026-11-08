#pragma once

#include <limits>

#include "FieldMacros.h"

// 拘束の柔らかさと最大力。既定値はエンジン側のConstraintTuningに合わせる。
struct ConstraintTuningDocument
{
public:
	bool operator==(const ConstraintTuningDocument&) const = default;

public:
	float compliance{ 0.0f };
	float maxForce{ (std::numeric_limits<float>::max)() };

	MD_DOCUMENT(
		ConstraintTuningDocument,
		"ConstraintTuning",

		MD_FIELD("Compliance", compliance,
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.001f),
			MD_GUI_FORMAT("%.6g")),

		MD_FIELD("MaxForce", maxForce,
			MD_GUI_LABEL("Max Force"),
			MD_GUI_MIN(0.0f),
			MD_GUI_FORMAT("%.6g"))
	);
};