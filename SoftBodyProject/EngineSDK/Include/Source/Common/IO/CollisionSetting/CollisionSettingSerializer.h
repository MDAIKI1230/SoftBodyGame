#pragma once

#include <filesystem>

#include "CollisionSettingDocument.h"

class CollisionSettingSerializer
{
public:
	static constexpr const char* DEFAULT_PATH{ "Res/Data/Setting/CollisionSetting.json" };

	static bool Load(CollisionSettingDocument& _output, std::filesystem::path _path);
	static bool Save(const CollisionSettingDocument _document, std::filesystem::path _path);
};