#pragma once

#include "StorageAccessorsMacros.h"

#include "PhysicsStorageBase.h"

#include "CharacterControllerID.h"
#include "CharacterControllerSlot.h"

class CharacterControllerStorage :public PhysicsStorageBase<CharacterControllerID, CharacterControllerSlot>
{
	MD_STORAGE_READ_WRITE_COLUMN()
};
