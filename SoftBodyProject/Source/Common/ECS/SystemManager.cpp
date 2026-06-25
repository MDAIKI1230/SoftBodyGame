#include "SystemManager.h"


void SystemManager::Update(WorldStorage* _worldStorage)
{
	// 更新
	for (int i{ 0 }; i < updateSystems.size(); i++)
	{
		updateSystems[i]->Update(this);
	}
}


void SystemManager::FixedUpdate(WorldStorage* _worldStorage)
{
	for (int i{ 0 }; i < fixedUpdateSystems.size(); i++)
	{
		fixedUpdateSystems[i]->FixedUpdate(this);
	}
}


void SystemManager::Render(WorldStorage* _worldStorage)
{
	// 更新
	for (int i{ 0 }; i < renderingSystems.size(); i++)
	{
		renderingSystems[i]->Draw(this);
	}
}

void SystemManager::AddSystem(std::unique_ptr<UpdateSystem> _system)
{
	// 入れる位置を探す
	auto it = std::lower_bound(
		updateSystems.begin(),
		updateSystems.end(),
		_system,
		[](const std::unique_ptr<UpdateSystem>& a, const std::unique_ptr<UpdateSystem>& b)
		{
			return a->GetPriority() > b->GetPriority();
		});

	// その位置に挿入
	updateSystems.insert(it, std::move(_system));
}

void SystemManager::AddSystem(std::unique_ptr<FixedUpdateSystem> _system)
{
	// 入れる位置を探す
	auto it = std::lower_bound(
		fixedUpdateSystems.begin(),
		fixedUpdateSystems.end(),
		_system,
		[](const std::unique_ptr<FixedUpdateSystem>& a, const std::unique_ptr<FixedUpdateSystem>& b)
		{
			return a->GetPriority() > b->GetPriority();
		});

	// その位置に挿入
	fixedUpdateSystems.insert(it, std::move(_system));
}

void SystemManager::AddSystem(std::unique_ptr<RenderingSystem> _system)
{
	// 入れる位置を探す
	auto it = std::lower_bound(
		renderingSystems.begin(),
		renderingSystems.end(),
		_system,
		[](const std::unique_ptr<RenderingSystem>& a, const std::unique_ptr<RenderingSystem>& b)
		{
			return a->GetPriority() > b->GetPriority();
		});

	// その位置に挿入
	renderingSystems.insert(it, std::move(_system));
}
