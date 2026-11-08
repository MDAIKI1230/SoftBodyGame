#pragma once

#include "ResourceOwnerHandle.h"

class LoadingScreen
{
public:
	LoadingScreen();
	virtual void Prepare() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	bool CanTerminate() { return canTerminate; }
	ResourceOwnerHandle GetResourceOwnerID() { return resourceOwnerID; }
	virtual ~LoadingScreen() = default;
protected:
	bool canTerminate{ false };
private:
	friend class SceneBase;
	ResourceOwnerHandle resourceOwnerID;
};