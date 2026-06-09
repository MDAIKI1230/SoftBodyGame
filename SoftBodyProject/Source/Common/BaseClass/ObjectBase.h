#pragma once

class ObjectBase
{
public:
	virtual void Update() = 0;
	virtual void FixedUpdate() = 0;
	// 描画はレンダリングコンポーネントで対応するためなし
	// 仮想デストラクタ
	virtual ~ObjectBase();
};