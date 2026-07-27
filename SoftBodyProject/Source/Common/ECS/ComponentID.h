#pragma once

#include "GenerationalID.h"

template<class COMPONENT>
struct ComponentTag {};

template<class COMPONENT>
using ComponentID = GenerationalID<ComponentTag<COMPONENT>>;
