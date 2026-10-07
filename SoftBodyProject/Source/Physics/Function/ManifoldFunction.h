#pragma once

#include "BoxBoxContactInfo.h"
#include "ContactInfo.h"

#include "CollisionManifoldBuffer.h"
#include "PhysicsTransformStorage.h"

namespace ManifoldFunction
{
	void AddFaceAManifold(
		PhysicsTransformStorage* _transformStorage,
		PhysicsTransformID _transformA, const Vector3& _positionA, const Vector3* _candidateAxisA, const float* _halfsA, ContactInfo& _outputContactA,
		PhysicsTransformID _transformB, const Vector3& _positionB, const Vector3* _candidateAxisB, const float* _halfsB, ContactInfo& _outputContactB,
		const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);
	void AddFaceBManifold(
		PhysicsTransformStorage* _transformStorage,
		PhysicsTransformID _transformA, const Vector3& _positionA, const Vector3* _candidateAxisA, const float* _halfsA, ContactInfo& _outputContactA,
		PhysicsTransformID _transformB, const Vector3& _positionB, const Vector3* _candidateAxisB, const  float* _halfsB, ContactInfo& _outputContactB,
		const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);
	void AddEdgeManifold(
		PhysicsTransformStorage* _transformStorage,
		PhysicsTransformID _transformA, const Vector3& _positionA, const Vector3* _candidateAxisA, const float* _halfsA, ContactInfo& _outputContactA,
		PhysicsTransformID _transformB, const Vector3& _positionB, const Vector3* _candidateAxisB, const float* _halfsB, ContactInfo& _outputContactB,
		const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);

	// 衝突点を出すのに必要な点を出してくれる関数
	std::vector<Vector3> GenerateFaceContact(
		const Vector3& _refarencePos, const Vector3* const _refarenceAxis, const  float* _refarenceHalfs,
		const Vector3& _incidentPos, const Vector3* const _incidentAxis, const float* _incidentHalfs,
		const Vector3& _faceNormal, const Vector3& _faceCenter, int _refarenceIndex);
	// 面の頂点をある面内に収める関数
	std::vector<Vector3> ClipFace(const std::vector<Vector3>& _input, const Vector3& _planeNormal, float _planeOffset);
	// ManifoldにPointを追加するときに近い点を排除してくれる追加
    void AddUniquePoint(Manifold& _manifold, const ContactPoint& _point);
	
	void BoxBox(
		PhysicsTransformStorage* _transformStorage,
		PhysicsTransformID _transformA, const Vector3& _positionA, const Vector3* _candidateAxisA, const float* _halfsA, ContactInfo& _outputContactA,
		PhysicsTransformID _transformB, const Vector3& _positionB, const Vector3* _candidateAxisB, const float* _halfsB, ContactInfo& _outputContactB,
		const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer);

	// 出力用情報作成関数(法線方向がA→Bで作られている前提で動くことに注意)
	void CreateContactInfo(ContactInfo& _outputA, ContactInfo& _outputB, const Manifold& _manifold);
}
