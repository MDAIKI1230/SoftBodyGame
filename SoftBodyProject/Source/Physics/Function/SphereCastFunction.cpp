#include "SphereCastFunction.h"

// SphereRayと球の当たり判定
bool SphereCastFunction::Sphere(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Vector3& _scale, float _radius)
{

}
// SphereRayとボックスの当たり判定
bool SphereCastFunction::Box(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, const Vector3& _scale, const Vector3& _halfScale)
{

}
// SphereRayとカプセルの当たり判定
bool SphereCastFunction::Capsule(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, float _radius, float _height)
{

}
// SphereRayとAABBの当たり判定
bool SphereCastFunction::AABB(const SphereRay& _ray, const Vector3& _min, const Vector3& _max, float _currentMaxDistance, float& _tEnter)
{

}