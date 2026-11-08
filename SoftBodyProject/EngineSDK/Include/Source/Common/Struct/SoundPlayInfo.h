#pragma once

struct SoundPlayInfo
{
	float volume{ 1.0f }; // 0.0～1.0
	float pan{ 0.0f };    // -1.0～1.0
	float pitch{ 1.0f };
	bool loop{ false };
};