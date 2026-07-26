#pragma once

#include "GenerationalID.h"

struct ComponentTag;

template<class COMPONENT>
struct ComponentID
{
	GenerationalID<ComponentTag>;
};
