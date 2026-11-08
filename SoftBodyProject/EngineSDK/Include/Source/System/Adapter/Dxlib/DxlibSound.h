#pragma once

#include <unordered_set>

#include "ISound.h"
#include "ResourceStorage.h"

class DxlibSound :public ISound
{
protected:
	// 音源リソース
	SoundHandle LoadSound(const std::string& _fileName) override;
	void DeleteSound(SoundHandle _sound) override;

	// 再生
	SoundHandle Play(SoundHandle _sound, const SoundPlayInfo& _info) override;

	void Stop(SoundHandle _handle) override;
	void Pause(SoundHandle _handle) override;
	void Resume(SoundHandle _handle) override;

	// 再生状態
	bool IsPlaying(SoundHandle _handle) const override;

	// 再生中の音への設定
	void SetVolume(SoundHandle _handle, float _volume) override;

	void SetPan(SoundHandle _handle, float _pan) override;

	void SetPitch(SoundHandle _handle, float _pitch) override;

	void StopAll() override;
private:
	ResourceStorage<SoundHandle, int> soundStorage;

	// StopAll用
	std::unordered_set<SoundHandle> soundHandles;
};