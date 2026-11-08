#pragma once

#include "PropertySnapshot.h"
#include "PropertyTarget.h"

/*
	値の変更履歴を残しておくための構造体
	Targetで対象を識別し、変更前後の値をSnapshotで保持する。
*/
struct PropertyChangeRecord
{
	PropertyTarget target;

	PropertySnapshot before;
	PropertySnapshot after;
};