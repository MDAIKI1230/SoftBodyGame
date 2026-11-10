#pragma once

#include "MonoBehaviour.h"

class ToyCrane : public MonoBehaviour
{
public:
	using MonoBehaviour::MonoBehaviour;

	void Start() override;
	void Update() override;
};