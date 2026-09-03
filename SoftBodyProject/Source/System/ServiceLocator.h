#pragma once

#include "IRenderer.h"
#include "IInput.h"
#include "InputSystem.h"
#include "IGPUConnecter.h"
#include "TimeManager.h"
#include "ResourceManager.h"

class ServiceLocator
{
public:
	// 取得用関数
	// static JobSystem* GetJobSystem() { return jobSystem; }
	static IRenderer* GetRenderer() { return renderer; }
	static InputSystem* GetInputSytem() { return inputSystem; }
	static IGPUConnecter* GetGPUConnecter() { return gpuConnecter; }
	static TimeManager* GetTimeManager() { return timeManager; }
	static ResourceManager* GetResourceManager() { return resourceManager; }

	// セット用関数
	// static void SetJobSystem(JobSystem* _jobSystem) { jobSystem = _jobSystem; }
	static void SetRenderer(IRenderer* _renderer) { renderer = _renderer; }
	static void SetInputSystemt(InputSystem* _inputSystem) { inputSystem = _inputSystem; }
	static void SetGPUConnecter(IGPUConnecter* _gpuConnecter) { gpuConnecter = _gpuConnecter; }
	static void SetTimeManager(TimeManager* _timeManager) { timeManager = _timeManager; }
	static void SetResourceManager(ResourceManager* _resourceManager) { resourceManager = _resourceManager; }
private:
	// static JobSystem* jobSystem;
	inline static IRenderer* renderer{ nullptr };
	inline static IInput* input{ nullptr };
	inline static InputSystem* inputSystem{ nullptr };
	inline static IGPUConnecter* gpuConnecter{ nullptr };
	inline static TimeManager* timeManager{ nullptr };
	inline static ResourceManager* resourceManager{ nullptr };
};
