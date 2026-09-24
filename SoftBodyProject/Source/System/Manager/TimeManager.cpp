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

// --- 外に公開する関数の実装部分 ---

// 更新用関数
void TimeManager::UpdateImpl()
{
	pastTime = nowTime;
	nowTime = std::chrono::steady_clock::now();

	deltaTime = std::chrono::duration<double>(nowTime - pastTime).count();
	fixedTimer += std::min(fixedTimer + deltaTime, FIXED_TIME * MAX_FIXED_UPDATES_PER_FRAME);
}

// 次のフレーム始まるの待ち関数
void TimeManager::WaitNextFrameImpl()
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

// 経過時間取得
float TimeManager::GetDeltaTimeImpl()
{
	return static_cast<float>(deltaTime);
}

// 経過時間取得
float TimeManager::GetFixedDeltaTimeImpl()
{
	return static_cast<float>(FIXED_TIME);
}

// 描画と物理の補間用α値
float TimeManager::GetFixedAlphaImpl()
{
	return static_cast<float>(fixedTimer / FIXED_TIME);
}

// FixedUpdateを呼び出すタイミングか
bool TimeManager::IsFixedUpdateTimeImpl()
{
	if (fixedTimer >= FIXED_TIME)
	{
		fixedTimer -= FIXED_TIME;
		return true;
	}
	return false;
}

// リセット関数シーン遷移後に呼ぶ
void TimeManager::ResetTimeImpl()
{
	nextFrameTime = std::chrono::steady_clock::now();
	nowTime = std::chrono::steady_clock::now();
	pastTime = std::chrono::steady_clock::now();
}