#pragma once

#include "Base/PanelBase.h"

class CollisionSettingPanel :public PanelBase
{
public:
	CollisionSettingPanel(EditorDocumentService& _documentService);

private:
	void DrawInternal() override;
private:
	std::uint8_t newCategoryBit{ 4 };
	std::string newCategoryName{ "NEW_TYPE" };
	float warningUntil{ 0.0 };
	float warningTimer{ 0.0f };
};