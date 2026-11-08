#pragma once

#include <string>

#include "Base/PanelBase.h"

class InspectorPanel :public PanelBase
{
public:
	InspectorPanel(EditorDocumentService& _documentService);
private:
	void DrawInternal() override;
private:
	std::string searchText;
};