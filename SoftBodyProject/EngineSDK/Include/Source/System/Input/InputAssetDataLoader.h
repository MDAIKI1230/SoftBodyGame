#pragma once

#include <string>

#include "Struct/InputActionAssetData.h"

namespace InputAssetDataLoader
{
	bool LoadAsset(std::string _path, InputActionAssetData& _output);
};
