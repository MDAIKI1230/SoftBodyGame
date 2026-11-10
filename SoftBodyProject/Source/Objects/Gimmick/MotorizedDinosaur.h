#pragma once

#include "MonoBehaviour.h"

class MotorizedDinosaur : public MonoBehaviour
{
public:
	using MonoBehaviour::MonoBehaviour;

	void Start() override;
	void Update() override;
};