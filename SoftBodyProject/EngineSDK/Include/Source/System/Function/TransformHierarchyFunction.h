#pragma once

#include "TransformComponentStorage.h"

namespace TransformHierarchyFunction
{
	void CalcTransformHierarchy(TransformComponentStorage* _storage);
	bool CalcTransform(TransformComponentStorage* _storage, EntityID _target);
}