#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "FieldMacros.h"
#include "MDMath.h"
#include "SceneComponentData.h"

struct ModelRendererComponentData : public SceneComponentData
{
public:
	std::string GetName() override { return "ModelRenderer"; }
public:
	std::string modelPath{};

	MD_DOCUMENT(
		ModelRendererComponentData, "ModelRenderer",
		MD_FIELD("ModelPath", modelPath)
	);
};