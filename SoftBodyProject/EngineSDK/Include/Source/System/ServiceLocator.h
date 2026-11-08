#pragma once

#include "IRenderer.h"
#include "IGPUConnecter.h"
#include "ISound.h"
#include "ISystem.h"

class ServiceLocator
{
	friend class ResourceManager;
	friend class Renderer;
	friend class LightManager;
	friend class GameApplication;
	friend class EditorApplication;
	friend class SoundPlayer;
	friend class EngineRuntime;

private:
	// 取得用関数
	static IRenderer* GetRenderer() { return renderer; }
	static IGPUConnecter* GetGPUConnecter() { return gpuConnecter; }
	static ISound* GetSound() { return sound; }
	static ISystem* GetSystem() { return system; }

	// セット用関数
	static void SetRenderer(IRenderer* _renderer) { renderer = _renderer; }
	static void SetGPUConnecter(IGPUConnecter* _gpuConnecter) { gpuConnecter = _gpuConnecter; }
	static void SetSound(ISound* _sound) { sound = _sound; }
	static void SetSystem(ISystem* _system) { system = _system; }
private:
	inline static IRenderer* renderer{ nullptr };
	inline static IGPUConnecter* gpuConnecter{ nullptr };
	inline static ISound* sound{ nullptr };
	inline static ISystem* system{ nullptr };
};