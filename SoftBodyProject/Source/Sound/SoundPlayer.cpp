#include "ServiceLocator.h"
#include "SoundPlayer.h"

void SoundPlayer::UpdateImpl()
{
	ISound* sound{ ServiceLocator::GetSound() };
	if (sound == nullptr)
	{
		return;
	}

	for (auto it{ playingSounds.begin() }; it != playingSounds.end();)
	{
		const SoundHandle handle{ *it };

		if (pausedSounds.contains(handle) || sound->IsPlaying(handle))
		{
			++it;
			continue;
		}

		sound->DeleteSound(handle);
		pausedSounds.erase(handle);
		it = playingSounds.erase(it);
	}
}

SoundHandle SoundPlayer::PlayImpl(SoundHandle _sound, const SoundPlayInfo& _info)
{
	ISound* sound{ ServiceLocator::GetSound() };
	if (sound == nullptr || !_sound.IsValid())
	{
		return{};
	}

	SoundHandle handle{ sound->Play(_sound, _info) };
	if (handle.IsValid()) playingSounds.insert(handle);
	return handle;
}

void SoundPlayer::StopImpl(SoundHandle _handle)
{
	if (!playingSounds.contains(_handle))
	{
		return;
	}

	ISound* sound{ ServiceLocator::GetSound() };
	if (sound == nullptr)
	{
		return;
	}
	sound->Stop(_handle);
	pausedSounds.erase(_handle);
}

void SoundPlayer::PauseImpl(SoundHandle _handle)
{
	if (!playingSounds.contains(_handle) || pausedSounds.contains(_handle)) return;

	ISound* sound{ ServiceLocator::GetSound() };
	if (sound == nullptr)
	{
		return;
	}
	sound->Pause(_handle);
	pausedSounds.insert(_handle);
}

void SoundPlayer::ResumeImpl(SoundHandle _handle)
{
	if (!pausedSounds.contains(_handle))
	{
		return;
	}

	ISound* sound{ ServiceLocator::GetSound() };
	if (sound == nullptr) return;

	sound->Resume(_handle);
	pausedSounds.erase(_handle);
}

bool SoundPlayer::IsPlayingImpl(SoundHandle _handle) const
{
	ISound* sound{ ServiceLocator::GetSound() };
	return sound != nullptr && sound->IsPlaying(_handle);
}

void SoundPlayer::SetVolumeImpl(SoundHandle _handle, float _volume)
{
	if (!playingSounds.contains(_handle))
	{
		return;
	}

	ISound* sound{ ServiceLocator::GetSound() };
	if (sound != nullptr) sound->SetVolume(_handle, _volume);
}

void SoundPlayer::SetPanImpl(SoundHandle _handle, float _pan)
{
	if (!playingSounds.contains(_handle))
	{
		return;
	}

	ISound* sound{ ServiceLocator::GetSound() };
	if (sound != nullptr) sound->SetPan(_handle, _pan);
}

void SoundPlayer::SetPitchImpl(SoundHandle _handle, float _pitch)
{
	if (!playingSounds.contains(_handle))
	{
		return;
	}

	ISound* sound{ ServiceLocator::GetSound() };
	if (sound != nullptr) sound->SetPitch(_handle, _pitch);
}

void SoundPlayer::StopAllImpl()
{
	ISound* sound{ ServiceLocator::GetSound() };
	if (sound == nullptr)
	{
		return;
	}

	sound->StopAll();

	for (SoundHandle handle : playingSounds)
	{
		sound->DeleteSound(handle);
	}

	playingSounds.clear();
	pausedSounds.clear();
}