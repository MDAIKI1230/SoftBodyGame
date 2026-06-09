#pragma once

#include "SparseSetStorageBase.h"

#include "TransformComponent.h"

class TransformComponentStorage :public SparseSetStorageBase<TransformComponent>
{
public:
	TransformComponentStorage()
	{

		sparseSet.Reserve(50);
	}
};