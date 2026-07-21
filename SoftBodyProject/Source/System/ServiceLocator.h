#pragma once

#include "IRenderer.h"
#include "IInput.h"
#include "IGPUConnecter.h"
#include "TimeManager.h"

class ServiceLocator
{
public:
	// 取得用関数
	// static JobSystem* GetJobSystem() { return jobSystem; }
	static IRenderer* GetRenderer() { return renderer; }
	static IInput* GetInput() { return input; }
	static IGPUConnecter* GetIGPUConnecter() { return gpuConnecter; }
	static TimeManager* GetTimeManager() { return timeManager; }

	// セット用関数
	// static void SetJobSystem(JobSystem* _jobSystem) { jobSystem = _jobSystem; }
	static void SetRenderer(IRenderer* _renderer) { renderer = _renderer; }
	static void SetInput(IInput* _input) { input = _input; }
	static void SetGPUConnecter(IGPUConnecter* _gpuConnecter) { gpuConnecter = _gpuConnecter; }
	static void SetTimeManager(TimeManager* _timeManager) { timeManager = _timeManager; }
private:
	// static JobSystem* jobSystem;
	inline static IRenderer* renderer{ nullptr };
	inline static IInput* input{ nullptr };
	inline static IGPUConnecter* gpuConnecter{ nullptr };
	inline static TimeManager* timeManager{ nullptr };
};
