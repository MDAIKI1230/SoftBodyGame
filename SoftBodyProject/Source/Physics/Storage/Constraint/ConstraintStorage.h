#pragma once

#include "StorageAccessorsMacros.h"

#include "ConstraintID.h"
#include "ConstraintSlot.h"

#include "PhysicsStorageBase.h"

#include "PointConstraint.h"
#include "DistanceConstraint.h"
#include "HingeConstraint.h"
#include "AngleLimitPointConstraint.h"
#include "AngleLimitHingeConstraint.h"
#include "LimitedBallJointConstraint.h"
#include "JointDriveConstraint.h"
#include "PointOnLineConstraint.h"
#include "SliderConstraint.h"
#include "LimitedPointOnLineConstraint.h"
#include "LimitedSliderConstraint.h"

#include "Base/ConstraintStorageBase.h"

#define CONSTRAINT_SCCESSORS(ConstraintType, EnumType, MemberName, ONE)\
private:\
std::unique_ptr<ConstraintStorageBase<ConstraintType>> MemberName;\
MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, ConstraintType##ID, MemberName, ID);\
MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, ConstraintType, ConstraintType, MemberName, Constraint);\
/*\
*  対応拘束作成\
*/\
template<class T>\
requires std::same_as<T, ConstraintType>\
ConstraintID Create(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation = Quaternion::IDENTITY)\
{\
	ConstraintID id{ CreateID(EnumType,MemberName->CountConstraint(),_entity,_transformID) };\
	\
	ConstraintType constraint;\
	constraint.ownerEndPoint = EndPointFrame{ _transformID ,_localOffset ,_localRotation };\
	\
	MemberName->Add(id, constraint);\
	\
	transformMap[_transformID].push_back(id);\
	\
	return id;\
}\
/*\
*  EndPointを追加する\
*/\
template<class T>\
requires std::same_as<T, ConstraintType>\
void AddEndPoint(ConstraintID _id, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation = Quaternion::IDENTITY)\
{\
	if constexpr(ONE)\
	{\
		Edit##ConstraintType(_id).otherEndPoint = EndPointFrame{ _transformID, _localOffset, _localRotation };\
	}\
	else\
	{\
		Edit##ConstraintType(_id).endPoints.emplace_back(_transformID, _localOffset, _localRotation);\
	}\
}\
/*\
*  対応TransformIDのEndPointを除外する\
*/\
template<class T>\
requires (std::same_as<T, ConstraintType> && ONE)\
void RemoveEndPoint(ConstraintID _id)\
{\
	Edit##ConstraintType(_id).RemoveEndPoint();\
}\
/*\
*  対応TransformIDのEndPointを除外する\
*/\
template<class T>\
requires (std::same_as<T, ConstraintType> && !ONE)\
void RemoveEndPoint(ConstraintID _id, PhysicsTransformID _transformID)\
{\
	Edit##ConstraintType(_id).RemoveEndPoint(_transformID);\
}\
/*\
	拘束からEndPointすべて除外\
*/ \
template<class T>\
requires (std::same_as<T, ConstraintType> && !ONE)\
void RemoveEndPointOtherAll(ConstraintID _id)\
{\
	Edit##ConstraintType(_id).RemoveEndpointOtherAll();\
}\
/*\
*  相手のEndPointすべて取得\
*/\
template<class T>\
requires (std::same_as<T, ConstraintType> && !ONE)\
std::span<const EndPointFrame> GetOtherEndPoints(ConstraintID _id)\
{\
	return std::span{ Get##ConstraintType(_id).endPoints };\
}\
/*\
*  相手のEndPoint取得\
*/\
template<class T>\
requires (std::same_as<T, ConstraintType> && ONE)\
const EndPointFrame& GetOtherEndPoint(ConstraintID _id)\
{\
	return Get##ConstraintType(_id).otherEndPoint;\
}\
/*\
*  相手のEndPointすべて取得\
*/\
template<class T>\
requires (std::same_as<T, ConstraintType> && !ONE)\
std::span<EndPointFrame> EditOtherEndPoints(ConstraintID _id)\
{\
	return std::span{ Edit##ConstraintType(_id).endPoints };\
}\
/*\
*  相手のEndPoint取得\
*/\
template<class T>\
requires (std::same_as<T, ConstraintType> && ONE)\
EndPointFrame& EditOtherEndPoint(ConstraintID _id)\
{\
	return Edit##ConstraintType(_id).otherEndPoint;\
}\
/*\
*　自身のEndPoint取得\
*/\
template<class T>\
requires std::same_as<T, ConstraintType>\
const EndPointFrame& GetEndPoint(ConstraintID _id)\
{\
	return Get##ConstraintType(_id).ownerEndPoint;\
}\
/*\
*　自身のEndPoint変更\
*/\
template<class T>\
requires std::same_as<T, ConstraintType>\
void SetEndPoint(ConstraintID _id, EndPointFrame _endPoint)\
{\
	EndPointFrame& ownerEndPoint{ Edit##ConstraintType(_id).ownerEndPoint };\
	_endPoint.transformID = ownerEndPoint.transformID;\
	ownerEndPoint = _endPoint;\
}\
/*\
*　自身のEndPoint変更\
*/\
template<class T>\
requires std::same_as<T, ConstraintType>\
EndPointFrame& EditEndPoint(ConstraintID _id)\
{\
	return Edit##ConstraintType(_id).ownerEndPoint;\
}\
/*\
*　すべての対応EndPointをSpanで帰させる関数\
*/\
template<class T>\
requires std::same_as<T, ConstraintType>\
std::span<const EndPointFrame> GetOtherEndPointsSpan(ConstraintID _id)\
{\
	if constexpr (ONE)\
	{\
		const EndPointFrame& other = GetOtherEndPoint<T>(_id);\
		\
		/*\
		* 相手が設定されていなければ空\ 
		*/ \
		if (!other.transformID.IsValid())\
			return {};\
		\
		return { &other, 1 };\
	}\
	else\
	{\
		return GetOtherEndPoints<T>(_id);\
	}\
}

#define GET_ENDPOINT_CASE(T, Enum, Member, ONE) \
    case Enum: return GetEndPoint<T>(_id);
#define GET_OTHERENDPOINTS_CASE(T, Enum, Member, ONE) \
    case Enum: return GetOtherEndPointsSpan<T>(_id);

#define CONSTRAINT_LIST(X)\
/*\
	点拘束\
*/\
X(PointConstraint, ConstraintType::POINTS, pointConstraintStorage, false);\
/*\
	距離拘束\
*/\
X(DistanceConstraint, ConstraintType::DISTANCE, distanceConstraintStorage, false);\
/*\
	ヒンジ拘束\
*/\
X(HingeConstraint, ConstraintType::HINGE, hingeConstraintStorage, false);\
/*\
	角度制限付き点拘束\
*/\
X(AngleLimitPointConstraint, ConstraintType::ANGLE_LIMIT_POINT, angleLimitPointConstraintStorage, false);\
/*\
	角度制限付きヒンジ拘束_
*/\
X(AngleLimitHingeConstraint, ConstraintType::ANGLE_LIMIT_HINGE, angleLimitHingeConstraintStorage, false);\
/*\
	SwingTwist拘束\
*/\
X(LimitedBallJointConstraint, ConstraintType::LIMITED_BALL_JOINT, limitedBallJointConstraintStorage, false);\
/*\
	関節駆動拘束\
*/\
X(JointDriveConstraint, ConstraintType::JOINT_DRIVE, jointDriveConstraintStorage, true);\
/*\
	ある点を線上に制限する拘束\
*/\
X(PointOnLineConstraint, ConstraintType::POINT_ON_LINE, pointOnLineConstraintStorage, false);\
/*\
	スライダー拘束\
*/\
X(SliderConstraint, ConstraintType::SLIDER, sliderConstraintStorage, false);\
/*\
	距離制限のある、点を線上に制限する拘束\
*/\
X(LimitedPointOnLineConstraint, ConstraintType::LIMITED_POINT_ON_LINE, limitedPointOnLineConstraintStorage, false);\
/*\
	距離制限のある、スライダー拘束\
*/\
X(LimitedSliderConstraint, ConstraintType::LIMITED_SLIDER, limitedSliderConstraintStorage, false);\

class ConstraintStorage :public PhysicsStorageBase<ConstraintID, ConstraintSlot>
{
	CONSTRAINT_LIST(CONSTRAINT_SCCESSORS);
public:
	// コンストラクタ
	ConstraintStorage();

	// 種類がわからない時用のエンドポイント取得関数
	const EndPointFrame& GetEndPoint(ConstraintID _id);
	// 種類がわからない時用の対応エンドポイント取得関数
	std::span<const EndPointFrame> GetOtherEndPoints(ConstraintID _id);

	// 破棄
	void Destory(ConstraintID _id);

	// 種類取得
	ConstraintType GetType(ConstraintID _id) const;
	// TransformID
	PhysicsTransformID GetTransformID(ConstraintID _id) const;

	// TransformIDから対応した拘束取得
	bool TryGetConstraintIDFromTransformID(PhysicsTransformID _id, std::vector<ConstraintID>& _output);
private:
	// PhysicsTransformIDとの対応表
	std::unordered_map<PhysicsTransformID, std::vector<ConstraintID>> transformMap;
};
