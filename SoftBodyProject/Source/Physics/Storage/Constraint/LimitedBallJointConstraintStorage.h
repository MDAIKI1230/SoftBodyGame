#pragma once

#include "StorageAccessorsMacros.h"

#include "LimitedBallJointConstraint.h"

#include "ConstraintID.h"

class LimitedBallJointConstraintStorage
{
public:
	MD_STORAGE_READ_ONLY_COLUMN(ConstraintID, ID, ids);
	MD_STORAGE_READ_WRITE_COLUMN(LimitedBallJointConstraint, Constraint, constraints);
public:
	// 追加関数
	void Add(ConstraintID _id, LimitedBallJointConstraint& _constraint)
	{
		ids.push_back(_id);
		constraints.push_back(std::move(_constraint));
	}

	// 除外関数
	ConstraintID Remove(uint32_t _index)
	{
		if (ids.empty())
		{
			return ConstraintID{ 0, 0 };
		}

		size_t last = ids.size() - 1;
		ConstraintID movedId = ids[last];

		if (_index != last)
		{
			constraints[_index] = constraints[last];
			ids[_index] = ids[last];
		}

		constraints.pop_back();
		ids.pop_back();

		return movedId;
	}
};