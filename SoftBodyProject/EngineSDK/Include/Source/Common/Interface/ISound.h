#pragma once

#include <string>

#include "SoundHandle.h"
#include "SoundPlayInfo.h"

class ISound
{
	friend class ResourceManager;
	friend class SoundPlayer;

public:
	// コンストラクタ
	ISound() = default;
	// デストラクタ
	virtual ~ISound() = default;

protected:
	// 音源リソース
	virtual SoundHandle LoadSound(const std::string& _fileName) = 0;
	virtual void DeleteSound(SoundHandle _sound) = 0;

	// 再生
	virtual SoundHandle Play(SoundHandle _sound, const SoundPlayInfo& _info) = 0;

	virtual void Stop(SoundHandle _handle) = 0;
	virtual void Pause(SoundHandle _handle) = 0;
	virtual void Resume(SoundHandle _handle) = 0;

	// 再生状態
	virtual bool IsPlaying(SoundHandle _handle) const = 0;

	// 再生中の音への設定
	virtual void SetVolume(SoundHandle _handle, float _volume) = 0;

	virtual void SetPan(SoundHandle _handle, float _pan) = 0;

	virtual void SetPitch(SoundHandle _handle, float _pitch) = 0;

	virtual void StopAll() = 0;
};