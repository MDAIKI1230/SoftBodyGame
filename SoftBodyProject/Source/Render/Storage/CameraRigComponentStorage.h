#pragma once

#include "UniqueComponentStorageBase.h"

#include "HashTable.h"

#include "CameraRigComponent.h"

class CameraRigComponentStorage :public UniqueComponentStorageBase<CameraRigComponent, HashTable>
{

};
