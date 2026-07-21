#pragma once

class ComponentBase
{
public:
	// コンストラクタ
	ComponentBase(int _handle) :
		handle{ _handle }
	{
	}

	// ハンドル取得
	int GetHandle()
	{
		return handle;
	}

	// 仮想デストラクタ
	virtual ~ComponentBase() = default;
protected:
	int handle;
};
