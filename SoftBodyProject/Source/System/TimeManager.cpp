#include <thread>
#include "BaseConstants.h"

#include "TimeManager.h"

TimeManager::TimeManager() :
	timePerFps{ std::chrono::duration<double>(1.0 / Config::FPS) },
	nextFrameTime{ std::chrono::steady_clock::now() },
	nowTime{ std::chrono::steady_clock::now() },
	pastTime{ std::chrono::steady_clock::now() }
{
}

void TimeManager::Update()
{
	pastTime = nowTime;
	nowTime = std::chrono::steady_clock::now();

	deltaTime = std::chrono::duration<double>(nowTime - pastTime).count();
	fixedTimer += deltaTime;
}

void TimeManager::WaitNextFrame()
{
	// 目標終了時刻を求める
	nextFrameTime += std::chrono::duration_cast<std::chrono::steady_clock::duration>(timePerFps);

	auto now = std::chrono::steady_clock::now();

	// もし、目標終了時刻より遅いなら目標終了時刻を修正して終了
	if (now >= nextFrameTime)
	{
		nextFrameTime = now;
		return;
	}

	// スリープ
	if (now < nextFrameTime)
	{
		std::this_thread::sleep_until(nextFrameTime);
	}
}

float TimeManager::GetDeltaTime()
{
	return static_cast<float>(deltaTime);
}

float TimeManager::GetFixedDeltaTime()
{
	return static_cast<float>(FIXED_TIME);
}

bool TimeManager::IsFixedUpdateTime()
{
	if (fixedTimer >= FIXED_TIME)
	{
		fixedTimer -= FIXED_TIME;
		return true;
	}
	return false;
}