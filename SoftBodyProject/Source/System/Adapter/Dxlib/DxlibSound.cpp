#include <algorithm>
#include <cmath>
#include <DxLib.h>

#include "DxlibSound.h"

// 音源リソース
SoundHandle DxlibSound::LoadSound(const std::string& _fileName)
{
	const int previousType{ DxLib::GetCreateSoundDataType() };
	DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);

	const std::wstring fileName{ _fileName.begin(),_fileName.end() };
	const int nativeHandle{ DxLib::LoadSoundMem(fileName.c_str()) };

	DxLib::SetCreateSoundDataType(previousType);

	if (nativeHandle == -1)
	{
		return {};
	}

	SoundHandle handle{ soundStorage.Add(nativeHandle) };
	soundHandles.insert(handle);
	return handle;
}

void DxlibSound::DeleteSound(SoundHandle _sound)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_sound, nativeHandle))
	{
		return;
	}

	DxLib::StopSoundMem(nativeHandle);
	DxLib::DeleteSoundMem(nativeHandle);

	soundHandles.erase(_sound);
	soundStorage.Remove(_sound);
}

// 再生
SoundHandle DxlibSound::Play(SoundHandle _sound, const SoundPlayInfo& _info)
{
	int sourceHandle;
	if (!soundStorage.TryGet(_sound, sourceHandle))
	{
		return {};
	}

	const int nativeHandle{ DxLib::DuplicateSoundMem(sourceHandle) };
	if (nativeHandle == -1)
	{
		return {};
	}

	SoundHandle playHandle{ soundStorage.Add(nativeHandle) };
	soundHandles.insert(playHandle);

	SetVolume(playHandle, _info.volume);
	SetPan(playHandle, _info.pan);
	SetPitch(playHandle, _info.pitch);

	const int playType{ _info.loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK };
	if (DxLib::PlaySoundMem(nativeHandle, playType, TRUE) == -1)
	{
		DeleteSound(playHandle);
		return {};
	}

	return playHandle;
}

void DxlibSound::Stop(SoundHandle _handle)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_handle, nativeHandle))
	{
		return;
	}

	DxLib::StopSoundMem(nativeHandle);
	DxLib::SetSoundCurrentTime(0, nativeHandle);
}

void DxlibSound::Pause(SoundHandle _handle)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_handle, nativeHandle))
	{
		return;
	}

	DxLib::StopSoundMem(nativeHandle);
}

void DxlibSound::Resume(SoundHandle _handle)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_handle, nativeHandle))
	{
		return;
	}

	DxLib::PlaySoundMem(nativeHandle, DX_PLAYTYPE_BACK, FALSE);
}

// 再生状態
bool DxlibSound::IsPlaying(SoundHandle _handle) const
{
	int nativeHandle;
	auto& storage{ const_cast<ResourceStorage<SoundHandle, int>&>(soundStorage) };

	if (!storage.TryGet(_handle, nativeHandle))
	{
		return false;
	}
	return DxLib::CheckSoundMem(nativeHandle) == 1;
}

// 再生中の音への設定
void DxlibSound::SetVolume(SoundHandle _handle, float _volume)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_handle, nativeHandle))
	{
		return;
	}

	const float volume{ std::clamp(_volume,0.0f,1.0f) };
	DxLib::ChangeVolumeSoundMem(static_cast<int>(std::lround(volume * 255.0f)), nativeHandle);
}

void DxlibSound::SetPan(SoundHandle _handle, float _pan)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_handle, nativeHandle))
	{
		return;
	}

	const float pan{ std::clamp(_pan,-1.0f,1.0f) };
	DxLib::ChangePanSoundMem(static_cast<int>(std::lround(pan * 255.0f)), nativeHandle);
}

void DxlibSound::SetPitch(SoundHandle _handle, float _pitch)
{
	int nativeHandle;
	if (!soundStorage.TryGet(_handle, nativeHandle))
	{
		return;
	}

	DxLib::ResetFrequencySoundMem(nativeHandle);

	const int baseFrequency{ DxLib::GetFrequencySoundMem(nativeHandle) };
	if (baseFrequency <= 0)
	{
		return;
	}

	const float pitch{ std::clamp(_pitch,0.01f,4.0f) };
	DxLib::SetFrequencySoundMem(static_cast<int>(std::lround(baseFrequency * pitch)), nativeHandle);
}

void DxlibSound::StopAll()
{
	for (SoundHandle handle : soundHandles)
	{
		Stop(handle);
	}
}