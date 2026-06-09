#pragma once

#include "IRenderer.h"
#include "IInput.h"
#include "TimeManager.h"

class ServiceLocator
{
public:
	// 取得用関数
	// static JobSystem* GetJobSystem() { return jobSystem; }
	static IRenderer* GetRenderer() { return renderer; }
	static IInput* GetInputManager() { return input; }
	static TimeManager* GetTimeManager() { return timeManager; }

	// セット用関数
	// static void SetJobSystem(JobSystem* _jobSystem) { jobSystem = _jobSystem; }
	static void SetRenderer(IRenderer* _renderer) { renderer = _renderer; }
	static void SetInput(IInput* _input) { input = _input; }
	static void SetTimeManager(TimeManager* _timeManager) { timeManager = _timeManager; }
private:
	// static JobSystem* jobSystem;
	inline static IRenderer* renderer{ nullptr };
	inline static IInput* input{ nullptr };
	inline static TimeManager* timeManager{ nullptr };
};