#include "Simplex.h"

void Simplex::Add(const Vector3& _vec)
{
	vertices[size] = _vec;

	if (size <= 4)
	{
		size++;
	}
}

Vector3& Simplex::operator[](size_t _index)
{
	return vertices[_index];
}
