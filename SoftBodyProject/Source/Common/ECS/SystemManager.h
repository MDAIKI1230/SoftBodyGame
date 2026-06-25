#pragma once

#include "WorldStorage.h"

#include "UpdateSystem.h"
#include "FixedUpdateSystem.h"
#include "RenderingSystem.h"

class SystemManager
{
public:
	// 更新
	void Update(WorldStorage* _worldStorage);
	// 物理更新
	void FixedUpdate(WorldStorage* _worldStorage);
	// 描画
	void Render(WorldStorage* _worldStorage);

	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<UpdateSystem> _system);
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<RenderingSystem> _system);
	/// <summary>
	/// システムの追加(moveされる)
	/// </summary>
	/// <param name="_system">入れたいシステム</param>
	void AddSystem(std::unique_ptr<FixedUpdateSystem> _system);
private:
	// 更新系システム
	std::vector<std::unique_ptr<UpdateSystem>> updateSystems;
	// 物理ステップ更新系システム
	std::vector<std::unique_ptr<FixedUpdateSystem>> fixedUpdateSystems;
	// 描画系システム
	std::vector<std::unique_ptr<RenderingSystem>> renderingSystems;
};
