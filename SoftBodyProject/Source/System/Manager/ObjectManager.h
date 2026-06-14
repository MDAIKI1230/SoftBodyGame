#pragma once

#include <vector>

#include "ObjectBase.h"

class ObjectManager
{
public:
	void Update();
	void FixedUpdate();
private:
	std::vector<ObjectBase*> objects;
};