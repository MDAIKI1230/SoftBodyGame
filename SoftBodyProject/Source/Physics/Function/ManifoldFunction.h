#pragma once

#include "ContactInfo.h"

#include "CollisionManifoldBuffer.h"
#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

namespace ManifoldFunction
{
	void AddFaceAManifold(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);
	void AddFaceBManifold(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);
	void AddEdgeManifold(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);

	// 衝突点を出すのに必要な点を出してくれる関数
	std::vector<Vector3> GenerateFaceContact(
		Vector3& _refarencePos, Vector3* _refarenceAxis, float* _refarenceHalfs,
		Vector3& _incidentPos, Vector3* _incidentAxis, float* _incidentHalfs,
		Vector3& _faceNormal, Vector3& _faceCenter, int _refarenceIndex);
	// 面の頂点をある面内に収める関数
	std::vector<Vector3> ClipFace(const std::vector<Vector3>& _input, const Vector3& _planeNormal, float _planeOffset);
	// ManifoldにPointを追加するときに近い点を排除してくれる追加
    void AddUniquePoint(Manifold& _manifold, const ContactPoint& _point);


	
	void BoxBox(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);
}
