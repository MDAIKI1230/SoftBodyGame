#pragma once

#include <vector>

#include "InputActionContext.h"
#include "SoundPlayInfo.h"

#include "ObjectBase.h"
#include "EscapePart.h"
#include "Check.h"

class Escape :public ObjectBase
{
public:
	// コンストラクタ
	Escape(WorldStorage* _world, EntityID _entityID);

	// 更新関数
	void Update() override;

	// エスケープパーツの選択肢追加
	void AddSlecte(EscapePart* _parts);
	// チェックマークセット
	void SetCheck(Check* _check);
private:
	void EscapeInput(InputActionContext& _input);
	void SlectedUp(InputActionContext& _input);
	void SlectedDown(InputActionContext& _input);
	void SlectedStick(InputActionContext& _input);
	void Action(InputActionContext& _input);

	void ReCalcCheckPos();

	void ReCalcChild();
private:
	const Vector3 POSITION_OFFSET{ 60.0f,200.0f,0.0f };
	const Vector3 PARTS_COUNT_OFFSET{ 0.0f,220,0.0f };
	const Vector3 BASE_POS{ 478.0f,0.0f,0.0f };
	const Vector3 CHECK_OFFSET{ -85.0f,-100.0f,0.0f };
private:
	// エスケープ構成パーツ
	std::vector<EscapePart*> parts;
	// 選択されてるパーツ番号
	int selectPartIndex{ 0 };
	// 出てきたり出て行ったりの速度
	float speed{ 20.0f };
	// チェックマーク
	Check* check{ nullptr };
	// サウンドインフォ
	SoundPlayInfo soundInfo{ 0.5f,0.0f,1.0f,false };
};