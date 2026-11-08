#pragma once

#include <imgui.h>
#include <ImGuizmo.h>

#include "GizmoWidgets.h"

template<class T, class Draw>
bool GizmoWidgets::EditProperty(const PropertyTarget& _target, T& _value, Draw&& _draw)
{
	const std::string gizmoID{
		std::to_string(_target.objectID) + ":" +
		std::to_string(_target.componentID) + ":" +
		_target.fieldName
	};

	ImGuizmo::PushID(gizmoID.c_str());

	T beforeDraw{ _value };
	bool wasUsing{ ImGuizmo::IsUsing() };

	bool changed{ _draw(_value) };

	bool isUsing{ ImGuizmo::IsUsing() };

	if (history)
	{
		// 操作開始。最初の変更が入る前の値を保存する。
		if (!wasUsing && (isUsing || changed))
		{
			if (!history->BeginEdit(_target, beforeDraw))
			{
				_value = beforeDraw;

				// ドラック入力を一度無効化して、そのあと入力が出来る状態に戻している。
				ImGuizmo::Enable(false);
				ImGuizmo::Enable(true);
				ImGuizmo::PopID();
				return false;
			}
		}

		const bool operated{ wasUsing || isUsing || changed };

		if (operated &&
			ImGui::IsKeyPressed(ImGuiKey_Escape, false))
		{
			history->CancelEdit(_target);

			// ドラック入力を一度無効化して、そのあと入力が出来る状態に戻している。
			ImGuizmo::Enable(false);
			ImGuizmo::Enable(true);

			changed = !(_value == beforeDraw);
		}
		else if ((wasUsing || changed) && !isUsing)
		{
			history->CommitEdit(_target, _value);
		}
	}

	ImGuizmo::PopID();
	return changed;
}