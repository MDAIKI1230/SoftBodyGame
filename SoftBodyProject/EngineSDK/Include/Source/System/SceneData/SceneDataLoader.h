#pragma once

#include "SceneFileData.h"

class SceneDataLoader
{
public:
	static bool LoadJson(std::string _filePath, SceneFileData& _output);
};
