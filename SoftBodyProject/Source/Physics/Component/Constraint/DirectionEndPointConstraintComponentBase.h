#pragma once

#include "ConstraintID.h"

struct DirectionEndPointConstraintComponentBase
{
public:
	// IDを渡すコンストラクタ
	DirectionEndPointConstraintComponentBase(ConstraintID _id) :
		id{ _id }
	{
	}

	
private:
	ConstraintID id;
};