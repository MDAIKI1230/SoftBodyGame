#pragma once

#include "SceneFileData.h"

class SceneDataLoader
{
public:
	bool LoadJson(std::string _filePath, SceneFileData& _output);
};
