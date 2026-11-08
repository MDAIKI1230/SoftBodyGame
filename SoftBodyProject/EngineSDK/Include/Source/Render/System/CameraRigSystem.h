#pragma once

#include "LateUpdateSystem.h"

#include "CameraRigComponent.h"
#include "TransformComponent.h"

class CameraRigSystem :public LateUpdateSystem
{
public:
	void LateUpdate(WorldStorage* _worldStorage, EventManager* _eventManager) override;

private:
	void UpdateFPS(CameraRigComponent& cameraRig, TransformComponent& trans, const TransformComponent* _targetTrans);
	void UpdateTPS(CameraRigComponent& cameraRig, TransformComponent& trans, const TransformComponent* _targetTrans);
};
