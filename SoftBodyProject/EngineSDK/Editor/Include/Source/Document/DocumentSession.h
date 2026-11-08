#pragma once

#include "SceneDocument.h"
#include "CommandHistory.h"

struct DocumentSession
{
	SceneDocument document;
	CommandHistory command{ document };
};