#pragma once

#include <cstdint>

#include "FieldMacros.h"
#include "MDMath.h"

// 接続先の拘束点。SceneのObject IDで参照し、実行時のPhysicsTransformIDは保持しない。
struct EndPointDocument
{
public:
	// 配列の変更履歴で、拘束点の値を比較するために使う。
	bool operator==(const EndPointDocument&) const = default;

public:
	// 0は未設定。
	std::uint64_t objectID{ 0 };
	Vector3 localPosition{};
	// エンジンへ適用するときにQuaternionへ変換する。
	Vector3 localRotationDegrees{};
	float weight{ 1.0f };

	MD_DOCUMENT(
		EndPointDocument,
		"EndPoint",

		MD_FIELD("ObjectID", objectID,
			MD_GUI_LABEL("Object"),
			MD_GUI_OBJECT_REFERENCE()),

		MD_FIELD("LocalPosition", localPosition,
			MD_GUI_LABEL("Local Position")),

		MD_FIELD("LocalRotationDeg", localRotationDegrees,
			MD_GUI_LABEL("Local Rotation"),
			MD_GUI_UNIT("deg")),

		MD_FIELD("Weight", weight,
			MD_GUI_MIN(0.0f),
			MD_GUI_SPEED(0.01f))
	);
};