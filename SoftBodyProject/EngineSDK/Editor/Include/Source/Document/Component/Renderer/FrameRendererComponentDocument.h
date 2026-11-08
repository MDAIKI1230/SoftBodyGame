#pragma once

#include <cstdint>
#include <string>

#include "Base/ComponentDocument.h"
#include "ModelHandle.h"

struct FrameRendererComponentDocument :public ComponentDocument<FrameRendererComponentDocument>
{
public:
	using ComponentDocument::ComponentDocument;

	~FrameRendererComponentDocument() override;

	void Draw(const ObjectDocument& _object) override;
	// モデルを取得し、Frame名に対応する番号を解決する。
	void OnChange() override;
	bool LoadJson(const MDJson::Value& _json) override;
	bool AllowsMultiple() override;
public:
	std::string modelPath;
	std::string frameName;

	MD_DOCUMENT(
		FrameRendererComponentDocument,
		"FrameRenderer",
		MD_FIELD("ModelPath", modelPath,
			MD_GUI_LABEL("Model Name"),
			MD_GUI_FILE_PATH(),
			MD_GUI_FILE_NAME_ONLY(),
			MD_GUI_FILE_FILTER("*.mv1"),
			MD_GUI_INITIAL_DIRECTORY("Res/Model")),
		MD_FIELD("FrameName", frameName,
			MD_GUI_LABEL("Frame Name"))
	);
private:
	void ReleaseModel();
private:
	static constexpr std::uint32_t INVALID_FRAME_INDEX{ UINT32_MAX };

	// 現段階では単体のComponentが所有する。配置一式での共有は取り込み時に接続する。
	ModelHandle model;
	std::string loadedModelName;
	// Frame番号は保存せず、OnChangeで名前から解決する。
	std::uint32_t frameIndex{ INVALID_FRAME_INDEX };
};