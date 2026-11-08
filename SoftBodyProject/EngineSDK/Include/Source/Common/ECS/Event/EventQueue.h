#pragma once

#include <queue>

template<typename T>
class EventQueue
{
public:
	// 追加
	void Push(const T& _event)
	{
		write.push(_event);
	}

	// 取り出し
	bool Pop(T& out)
	{
		if (read.empty())
			return false;

		out = read.front();
		read.pop();
		return true;
	}

	// 空チェック
	bool IsEmpty()
	{
		return read.empty();
	}

	// スワップ
	void Swap()
	{
		std::swap(write, read);

		std::queue<T> empty;
		std::swap(write, empty);
	}

private:
	std::queue<T> read;
	std::queue<T> write;
};
