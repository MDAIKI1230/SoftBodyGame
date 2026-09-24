#pragma once

#include <unordered_set>

#include "SingletonMacros.h"
#include "SoundHandle.h"
#include "SoundPlayInfo.h"

class SoundPlayer
{
public:
	static void Update()
		SingletonPublicFunction(void, Update, (), ())

		static SoundHandle Play(SoundHandle _sound, const SoundPlayInfo& _info = {})
		SingletonPublicFunction(SoundHandle, Play, (SoundHandle _sound, const SoundPlayInfo& _info), (_sound, _info))

		static void Stop(SoundHandle _handle)
		SingletonPublicFunction(void, Stop, (SoundHandle _handle), (_handle))

		static void Pause(SoundHandle _handle)
		SingletonPublicFunction(void, Pause, (SoundHandle _handle), (_handle))

		static void Resume(SoundHandle _handle)
		SingletonPublicFunction(void, Resume, (SoundHandle _handle), (_handle))

		static bool IsPlaying(SoundHandle _handle)
		SingletonPublicFunction(bool, IsPlaying, (SoundHandle _handle) const, (_handle))

		static void SetVolume(SoundHandle _handle, float _volume)
		SingletonPublicFunction(void, SetVolume, (SoundHandle _handle, float _volume), (_handle, _volume))

		static void SetPan(SoundHandle _handle, float _pan)
		SingletonPublicFunction(void, SetPan, (SoundHandle _handle, float _pan), (_handle, _pan))

		static void SetPitch(SoundHandle _handle, float _pitch)
		SingletonPublicFunction(void, SetPitch, (SoundHandle _handle, float _pitch), (_handle, _pitch))

		static void StopAll()
		SingletonPublicFunction(void, StopAll, (), ())

private:
	SoundPlayer() = default;

	static SoundPlayer& Instance()
	{
		static SoundPlayer instance;
		return instance;
	}

private:
	std::unordered_set<SoundHandle> playingSounds;
	std::unordered_set<SoundHandle> pausedSounds;
};