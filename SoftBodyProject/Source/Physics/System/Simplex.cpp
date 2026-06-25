#include "Simplex.h"

void Simplex::Add(const Vector3& _vec)
{
	vertices[size] = _vec;

	if (size <= MAX_SIZE)
	{
		size++;
	}
}

void Simplex::Erase(int _index)
{
	if (_index < size)
	{
		return;
	}

	// 入れ替えてからのサイズ減らしで、実質消える
	Vector3 temp{ vertices[size - 1] };
	vertices[size - 1] = vertices[_index];
	vertices[_index] = temp;

	size--;
}

Vector3& Simplex::operator[](size_t _index)
{
	return vertices[_index];
}
