#pragma once

#include <utility>

#include "CommandHistory.h"


template<class T>
bool CommandHistory::BeginEdit(const PropertyTarget& _target, const T& _before)
{
	// 編集中なら、最初に保存した対象と値を保つ
	if (editing)
	{
		return false;
	}

	// 対象が存在するか確認
	if (!HasProperty(_target))
	{
		return false;
	}

	PropertyChangeRecord record;
	record.target = _target;
	record.before.Capture(_before);

	editingRecord = std::move(record);
	editing = true;

	return true;
}

template<class T>
void CommandHistory::CommitEdit(const PropertyTarget& _target, const T& _current)
{
	if (!editing || editingRecord.target != _target)
	{
		return;
	}

	if (editing)
	{
		editingRecord.after.Capture(_current);
		if (editingRecord.before != editingRecord.after)
		{
			Record(editingRecord);
		}
	}

	editingRecord = {};
	editing = false;
}