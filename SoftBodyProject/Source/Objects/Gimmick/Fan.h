#pragma once

#include "MonoBehaviour.h"

class Fan : public MonoBehaviour
{
public:
	using MonoBehaviour::MonoBehaviour;

	void Start() override;
	void Update() override;
};