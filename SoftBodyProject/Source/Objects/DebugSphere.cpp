#include "DebugSphere.h"

DebugSphere::DebugSphere(IWorld* world, int handle) :
	ObjectBase{ world,handle }
{
}

// --- 更新系 ---

void DebugSphere::Update()
{

}
void DebugSphere::FixedUpdate()
{

}

// --- 衝突系 ---

void DebugSphere::OnCollisionEnter()
{
	isCollision = true;
}
void DebugSphere::OnCollision()
{

}
void DebugSphere::OnCollisionExit()
{
	isCollision = false;
}