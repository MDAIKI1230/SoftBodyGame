#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "SceneComponentData.h"

struct FrameRendererComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "FrameRenderer"; }
public:
	std::string modelPath{};
	std::string frameName{};

	MD_DOCUMENT(
		FrameRendererComponentData, "FrameRenderer",
		MD_FIELD("ModelPath", modelPath),
		MD_FIELD("FrameName", frameName)
	);
};