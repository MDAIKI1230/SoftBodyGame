#pragma once

#include "ActiveRagdollComponent.h"

#include "InputActionContext.h"

#include "ObjectBase.h"

class Escape :public ObjectBase
{
public:
	// コンストラクタ
	Escape(WorldStorage* _world, EntityID _entityID);

	// --- 更新系 ---

	// 更新処理
	void Update() override;
	// 物理更新処理
	void FixedUpdate() override;

	// エスケープの選択肢追加
	void AddSlecte();
private:
	void SlectedMouse(InputActionContext _input);
	void SlectedButton(InputActionContext _input);
	void Action(InputActionContext _input);

private:
	
};
