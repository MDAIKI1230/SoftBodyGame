#pragma once

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Base/PanelBase.h"

class HierarchyPanel :public PanelBase
{
public:
	HierarchyPanel(EditorDocumentService& _documentService);

private:
	void DrawInternal() override;

	using ChildMap = std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>;
	using ObjectIDSet = std::unordered_set<std::uint64_t>;

	void DrawObject(
		std::uint64_t _objectID,
		const ChildMap& _children,
		const ObjectIDSet& _visibleObjects, ObjectIDSet& _visitedObjects,
		bool _draw, std::uint64_t& _droppedObjectID, std::uint64_t& _parentObjectID);

private:
	std::string searchText;
	std::uint64_t candidateSelectedObject{ 0 };
};