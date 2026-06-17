#pragma once

class ObjectBase
{
public:
	// --- 更新系 ---

	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;

	// --- 衝突系 ---
	
	virtual void OnCollisionEnter() = 0;
	virtual void OnCollision() = 0;
	virtual void OnCollisionExit() = 0;
	// 描画はレンダリングコンポーネントで対応するためなし
	// 仮想デストラクタ
	virtual ~ObjectBase() = default;
private:
	int handle;
};