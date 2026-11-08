#pragma once

#include "Base/ComponentDocument.h"
#include "ModelHandle.h"

struct ModelRendererComponentDocument :public ComponentDocument<ModelRendererComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	~ModelRendererComponentDocument() override;

	void Draw(const ObjectDocument& _object) override;
	void OnChange() override;
	bool LoadJson(const MDJson::Value& _json) override;
	bool AllowsMultiple() override;
public:
	std::string modelPath;

	MD_DOCUMENT(
		ModelRendererComponentDocument,
		"ModelRenderer",
		MD_FIELD("ModelPath", modelPath,
			MD_GUI_LABEL("Model Name"),
			MD_GUI_FILE_PATH(),
			MD_GUI_FILE_NAME_ONLY(),
			MD_GUI_FILE_FILTER("*.mv1"),
			MD_GUI_INITIAL_DIRECTORY("Res/Model"))
	);
private:
	void ReleaseModel();
private:
	ModelHandle model;
};