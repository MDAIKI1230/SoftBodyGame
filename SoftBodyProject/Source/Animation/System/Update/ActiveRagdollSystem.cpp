#include "TimeManager.h"

#include "PhysicsAPI.h"
#include "PhysicsComponentAPI.h"

#include "ActiveRagdollSystem.h"

void ActiveRagdollSystem::PrePhysicsFixedUpdate(SkeletonInstanceStorage* _skeletonStorage, RagdollStorage* _ragdollStorage, ActiveRagdollStorage* _activeRagdollStorage)
{
	for (ActiveRagdollID id : _activeRagdollStorage->GetIDRange())
	{
		ActiveRagdoll& activeRagdoll{ _activeRagdollStorage->EditActiveRagdoll(id) };

		RagdollID ragdollID{ _activeRagdollStorage->GetRagdollID(id) };

		const Ragdoll& ragdoll{ _ragdollStorage->GetRagdoll(ragdollID) };

		const PoseBuffer& targetPose{ _skeletonStorage->EditTargetPose(ragdoll.skeleton) };

		const CollisionFilter& ignoreFilter{ _ragdollStorage->GetIgnoreFilter(ragdollID) };

		UpdateGroundState(ragdoll, activeRagdoll, ignoreFilter);
		UpdateBodyState(ragdoll, activeRagdoll);
		UpdateControlState(activeRagdoll);
		UpdateRecovery(ragdoll, activeRagdoll);
		UpdateJump(ragdoll, activeRagdoll);
		UpdateMovement(ragdoll, activeRagdoll);
		UpdateUpright(ragdoll, activeRagdoll);
		UpdateBalance(ragdoll, activeRagdoll);
		UpdateJointDrive(ragdoll, activeRagdoll, targetPose);
	}
}

// 左右の足情報からRagdoll全体の接地状態を更新する関数
void ActiveRagdollSystem::UpdateGroundState(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll, const CollisionFilter& _ignoreFilter)
{
	// Role対応表から左右の足ボーンインデックスを取得する
	uint32_t leftFootBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::LEFT_FOOT)] };
	uint32_t rightFootBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::RIGHT_FOOT)] };

	FootGroundInfo& left{ _activeRagdoll.leftFootGround };
	FootGroundInfo& right{ _activeRagdoll.rightFootGround };

	// ジャンプ中なら、情報をリセットして終わる
	if (_activeRagdoll.jumpTime > 0.0f)
	{
		_activeRagdoll.jumpTime = std::max(
			_activeRagdoll.jumpTime - TimeManager::GetFixedDeltaTime(),
			0.0f);

		left = {};
		right = {};
		_activeRagdoll.isGrounded = false;
		_activeRagdoll.groundNormal = Vector3::UP;
		return;
	}

	// 左足の接地状態を更新する
	UpdateFootGroundState(_ragdoll, left, _activeRagdoll.settings, _ignoreFilter, leftFootBoneIndex);

	// 右足の接地状態を更新する
	UpdateFootGroundState(_ragdoll, right, _activeRagdoll.settings, _ignoreFilter, rightFootBoneIndex);

	// 歩行可能な地面へ接地している足を数える(ついでに法線も加算)
	Vector3 normal;
	int walkFoot{ 0 };
	if (left.state == RagdollGroundState::WALKABLE)
	{
		walkFoot++;
		normal += left.normal;
	}
	if (right.state == RagdollGroundState::WALKABLE)
	{
		walkFoot++;
		normal += right.normal;
	}

	// 一方でも歩行可能な地面へ接地していれば、
	// Ragdoll全体を接地状態にする
	if (walkFoot > 0)
	{
		_activeRagdoll.isGrounded = true;
		// 接地している足の地面法線を合成して正規化する
		normal.Normalize();

		_activeRagdoll.groundNormal = normal;
	}
	// 接地していなければ地面法線を上方向へ戻す
	else
	{
		_activeRagdoll.isGrounded = false;
		_activeRagdoll.groundNormal = Vector3::UP;
	}
}

// 指定した足の接地点・地面法線・傾斜状態を更新する関数
void ActiveRagdollSystem::UpdateFootGroundState(
	const Ragdoll& _ragdoll, FootGroundInfo& _footGroundInfo,
	const ActiveRagdollSetting& _setting, const CollisionFilter& _ignoreFilter, uint32_t _boneIndex)
{
	// ボーンインデックスと対応Bodyが有効か確認する
	// 無効なら非接地状態へ戻して終了する
	if (_boneIndex == UINT32_MAX)
	{
		_footGroundInfo.state = RagdollGroundState::AIRBORNE;
		_footGroundInfo.point = Vector3::ZERO;
		_footGroundInfo.normal = Vector3::UP;
		_footGroundInfo.distance = 0;

		return;
	}

	// 足Bodyの現在位置を取得する
	BodyID footBodyID{ _ragdoll.bodyLinks[_boneIndex].bodyID };
	ColliderID footColliderID{ _ragdoll.colliders[_boneIndex]};

	// 重力方向、またはワールド下方向をレイの方向にする
	Vector3 down{ PhysicsComponentAPI::GetGravity(footBodyID) };

	// 重力がほぼないならワールド下方向
	if (down.LengthSqr() <= MathConstants::EPSILON)
	{
		down = -Vector3::UP;
	}
	else
	{
		down.Normalize();
	}

	// Collider形状から重力方向の最下点を取得
	Vector3 solePoint{ PhysicsComponentAPI::Support(footColliderID, down) };

	// 少し上から開始して、足裏を確実に通過させる
	Vector3 origin{solePoint - down * _setting.groundProbeStartOffset};

	Ray ray{
		origin,
		down,
		_setting.groundProbeStartOffset + _setting.groundProbeDistance
	};

	// Ragdoll自身のCollider群を除外してRayCastする
	RayCastHitInfo hitInfo;
	if (!PhysicsAPI::RayCastHit(ray, hitInfo, _ignoreFilter))
	{
		// ヒットしなければ接地情報を初期化して終了する
		_footGroundInfo.state = RagdollGroundState::AIRBORNE;
		_footGroundInfo.point = Vector3::ZERO;
		_footGroundInfo.normal = Vector3::UP;
		_footGroundInfo.distance = 0;

		return;
	}

	// 接地点、地面法線、地面までの距離を保存する
	_footGroundInfo.point = hitInfo.point;
	_footGroundInfo.normal = hitInfo.normal;
	_footGroundInfo.distance = std::max(hitInfo.distance - _setting.groundProbeStartOffset, 0.0f);

	// 地面法線と上方向の内積を計算する
	float dot{ Vector3::Dot(hitInfo.normal,-down) };

	// 内積が歩行可能閾値以上ならWALKABLE、
	// それ未満ならSTEEP_SLOPEにする
	if (dot >= _setting.minGroundDot)
	{
		_footGroundInfo.state = RagdollGroundState::WALKABLE;
	}
	else
	{
		_footGroundInfo.state = RagdollGroundState::STEEP_SLOPE;
	}
}

// 腰の水平速度・胴体の傾き・重心などの現在状態を更新する関数
void ActiveRagdollSystem::UpdateBodyState(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll)
{
	// Role対応表から胴体/腰ボーンインデックスを取得する
	uint32_t torsoBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::TORSO)] };
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::PELVIS)] };

	// 胴体ボーンインデックスが有効か確認する
	// 無効なら直立度を最低値にして終了する
	if (torsoBoneIndex == UINT32_MAX || pelvisBoneIndex == UINT32_MAX)
	{
		_activeRagdoll.planarVelocity = Vector3::ZERO;
		_activeRagdoll.uprightDot = -1.0f;
		return;
	}

	// 胴体Body/腰Bodyの現在姿勢を取得する
	BodyID torsoBodyID{ _ragdoll.bodyLinks[torsoBoneIndex].bodyID };
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };

	// 腰Bodyの速度から地面に沿った水平速度を計算する
	_activeRagdoll.planarVelocity = CalculatePlanarVelocity(_activeRagdoll, pelvisBodyID);

	// 全Bodyの位置と質量からRagdoll全体の重心を計算する
	_activeRagdoll.centerOfMass = CalculateCenterOfMass(_ragdoll);

	// 胴体Bodyと腰Bodyの差を上方向とする
	Vector3 pelvisPosition{ PhysicsComponentAPI::GetRigidBodyPosition(pelvisBodyID) };

	Vector3 torsoPosition{ PhysicsComponentAPI::GetRigidBodyPosition(torsoBodyID) };

	Vector3 torsoUp{ torsoPosition - pelvisPosition };

	if (torsoUp.LengthSqr() <= MathConstants::EPSILON)
	{
		_activeRagdoll.uprightDot = -1.0f;
		return;
	}

	torsoUp.Normalize();

	// 接地中なら地面法線を基準の上方向にする
	// 非接地中ならワールド上方向を使用する(足の時点で計算済みのはず)
	// 胴体の上方向と基準上方向の内積から直立度を計算する
	_activeRagdoll.uprightDot = Vector3::Dot(_activeRagdoll.groundNormal, torsoUp);
}

// 接地状態や胴体の傾きから立位・空中・転倒状態を更新する関数
void ActiveRagdollSystem::UpdateControlState(ActiveRagdoll& _activeRagdoll)
{
	// 現在の接地状態と胴体の直立度を取得する

	// 現在の制御状態ごとに遷移条件を判定する
	switch (_activeRagdoll.controlState)
	{
	case ActiveRagdollControlState::STANDING:
		// 接地していなければ空中判定用タイマーを進める
		if (!_activeRagdoll.isGrounded)
		{
			_activeRagdoll.airborneTime += TimeManager::GetFixedDeltaTime();
		}
		// 再接地した場合は空中判定用タイマーをリセットする
		else
		{
			_activeRagdoll.airborneTime = 0.0f;
		}

		// 一定時間接地していなければAIRBORNEへ遷移する
		if (_activeRagdoll.airborneTime >= _activeRagdoll.settings.airborneDelay)
		{
			_activeRagdoll.controlState = ActiveRagdollControlState::AIRBORNE;
			_activeRagdoll.ResetTime();
			return;
		}

		// 接地中に直立度が転倒閾値を下回った場合は
		// 転倒判定用タイマーを進める
		if (_activeRagdoll.isGrounded &&
			_activeRagdoll.uprightDot <= _activeRagdoll.settings.fallenUprightDot)
		{
			_activeRagdoll.fallenTime += TimeManager::GetFixedDeltaTime();
		}
		// 直立度が戻った場合は転倒判定用タイマーをリセットする
		else
		{
			_activeRagdoll.fallenTime = 0.0f;
		}

		// 転倒状態が一定時間続いた場合はFALLENへ遷移する
		if (_activeRagdoll.fallenTime >= _activeRagdoll.settings.fallDelay)
		{
			_activeRagdoll.controlState = ActiveRagdollControlState::FALLEN;
			_activeRagdoll.ResetTime();
			return;
		}
		break;

	case ActiveRagdollControlState::AIRBORNE:
		// 非接地中はAIRBORNEを維持する
		if (!_activeRagdoll.isGrounded)
		{
			return;
		}
		// 接地した場合は直立度を確認する
		// 十分に直立していればSTANDINGへ遷移する
		if (_activeRagdoll.uprightDot >= _activeRagdoll.settings.recoveryUprightDot)
		{
			_activeRagdoll.controlState = ActiveRagdollControlState::STANDING;
			_activeRagdoll.ResetTime();
		}
		// 転倒閾値未満ならFALLENへ遷移する
		else
		{
			_activeRagdoll.controlState = ActiveRagdollControlState::FALLEN;
			_activeRagdoll.ResetTime();
		}
		break;

	case ActiveRagdollControlState::FALLEN:
		// 接地していて、直立度が復帰閾値を上回っている場合
		if (_activeRagdoll.isGrounded &&
			_activeRagdoll.uprightDot > _activeRagdoll.settings.recoveryUprightDot)
		{
			// 復帰判定用タイマーを進める
			_activeRagdoll.standingTime += TimeManager::GetFixedDeltaTime();
		}
		// 非接地、または再び傾いた場合
		else
		{
			// 復帰判定用タイマーをリセットする
			_activeRagdoll.ResetTime();
		}

		// 復帰可能な姿勢が一定時間続いた場合
		if (_activeRagdoll.standingTime >= _activeRagdoll.settings.recoveryDelay)
		{
			// STANDINGへ遷移する
			_activeRagdoll.controlState = ActiveRagdollControlState::STANDING;
			_activeRagdoll.ResetTime();
			return;
		}

		break;
	}
}

// 立ち上がるための力を加える関数
void ActiveRagdollSystem::UpdateRecovery(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll)
{
	// FALLENかつ接地中でなければ終了する
	if (_activeRagdoll.controlState != ActiveRagdollControlState::FALLEN ||
		!_activeRagdoll.isGrounded)
	{
		return;
	}
	// 腰BodyとRagdoll全体の質量を取得する
	// Role対応表から腰ボーンインデックスを取得する
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::PELVIS)] };
	// 腰ボーンインデックスが有効か確認する
	if (pelvisBoneIndex == UINT32_MAX)
	{
		return;
	}
	// 腰ボーンの対応ボディIDを取得
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };

	float totalMass{ 0.0f };
	// 総質量を計算
	for (const RagdollBodyLink& bodyLink : _ragdoll.bodyLinks)
	{
		BodyID bodyID{ bodyLink.bodyID };
		// 無効なBodyは計算対象から除外する
		if (!bodyID.IsValid())
		{
			continue;
		}
		// 総質量へBodyの質量を加算する
		totalMass += PhysicsComponentAPI::GetMass(bodyID);
	}
	// 重力と全質量から自重を打ち消すForceを作る
	Vector3 force{ -PhysicsComponentAPI::GetGravity(pelvisBodyID) * totalMass };

	float range{ _activeRagdoll.settings.recoveryUprightDot - _activeRagdoll.settings.fallenUprightDot };
	// 直立閾値と転倒敷地の間で線形保管し、立ち上がるほど、力を弱くする
	float uprightAmount{
		range > MathConstants::EPSILON
			? std::clamp(
				(_activeRagdoll.uprightDot - _activeRagdoll.settings.fallenUprightDot) / range,
				0.0f, 1.0f)
			: 0.0f
	};

	float recoveryAmount{ 1.0f - uprightAmount };

	PhysicsComponentAPI::AddForce(pelvisBodyID, force * _activeRagdoll.settings.recoveryLiftScale * recoveryAmount);
}

// ジャンプリクエストを処理し、ジャンプの力を加える関数
void ActiveRagdollSystem::UpdateJump(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll)
{
	// ジャンプ要求を確認して消費する
	if (!_activeRagdoll.jumpRequested)
	{
		return;
	}
	_activeRagdoll.jumpRequested = false;
	// STANDINGかつ接地中か確認する
	if (_activeRagdoll.controlState != ActiveRagdollControlState::STANDING ||
		!_activeRagdoll.isGrounded)
	{
		return;
	}
	// 重力の反対方向をジャンプ方向にする
	// Role対応表から腰ボーンインデックスを取得する
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::PELVIS)] };

	// 腰ボーンインデックスが有効か確認する
	if (pelvisBoneIndex == UINT32_MAX)
	{
		return;
	}

	// 腰Bodyの現在姿勢を取得する
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };

	Vector3 gravity{ PhysicsComponentAPI::GetGravity(pelvisBodyID) };
	float gravityLen{ gravity.Length() };
	// 重力がほぼないなら地面の終了
	Vector3 jumpDir{ -gravity };
	if (gravityLen <= MathConstants::EPSILON)
	{
		return;
	}

	jumpDir.Normalize();

	float totalMass{ 0.0f };
	float upwardMomentum{ 0.0f };

	for (const RagdollBodyLink& link : _ragdoll.bodyLinks)
	{
		if (!link.bodyID.IsValid())
		{
			continue;
		}

		float mass{ PhysicsComponentAPI::GetMass(link.bodyID) };
		totalMass += mass;
		upwardMomentum += Vector3::Dot(
			PhysicsComponentAPI::GetVelocity(link.bodyID),
			jumpDir) * mass;
	}

	// sqrt(2 * 重力加速度 * jumpHeight)で目標上向き速度を求める
	float jumpStrength{ std::sqrtf(2 * gravity.Length() * _activeRagdoll.settings.jumpHeight) };

	// 現在の上向き速度との差を求める
	float currentUpwardSpeed{ upwardMomentum / totalMass };
	float deltaSpeed{ std::max(jumpStrength - currentUpwardSpeed, 0.0f) };
	Vector3 deltaVelocity{ jumpDir * deltaSpeed };

	// 全Bodyへ同じ上向き速度差を加える
	for (const RagdollBodyLink& link : _ragdoll.bodyLinks)
	{
		if (link.bodyID.IsValid())
		{
			PhysicsComponentAPI::SetVelocity(
				link.bodyID,
				PhysicsComponentAPI::GetVelocity(link.bodyID) + deltaVelocity);
		}
	}
	
	// jumpTimeへ接地無視時間を設定する
	_activeRagdoll.ResetTime();
	_activeRagdoll.jumpTime = _activeRagdoll.settings.jumpGroundIgnoreTime;
	// isGroundedをfalseにする
	_activeRagdoll.isGrounded = false;
	// AIRBORNEへ遷移する
	_activeRagdoll.controlState = ActiveRagdollControlState::AIRBORNE;
}

// 入力から目標速度を作り、移動または減速用のForceを加える関数
void ActiveRagdollSystem::UpdateMovement(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll)
{
	// Role対応表から腰ボーンインデックスを取得する
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::PELVIS)] };

	// 腰ボーンインデックスが有効か確認する
	if (pelvisBoneIndex == UINT32_MAX)
	{
		return;
	}

	// 腰Bodyの現在姿勢を取得する
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };

	// 移動入力の長さを取得して0～1へ収める
	float inputLen{ _activeRagdoll.moveInput.Length() };
	// 入力の有無
	bool isInput{ false };
	// 目標速度ベクトル
	Vector3 targetVelocity{ Vector3::ZERO };
	if (inputLen > MathConstants::EPSILON)
	{
		// 接地中なら地面法線を移動平面の法線にする
		// 非接地中ならワールド上方向を使用する(すでに計算済み)

		// 移動入力を移動平面へ射影する
		Vector3 moveInputProjection{ _activeRagdoll.moveInput -
			(_activeRagdoll.groundNormal *
				Vector3::Dot(
					_activeRagdoll.groundNormal,
					_activeRagdoll.moveInput)) };

		// 射影後の入力が十分な長さを持つか確認する
		// 入力がある場合は、入力方向・入力強度・最大速度から
		// 目標移動速度を計算する
		// 入力がない場合は目標移動速度をゼロにして減速させる
		float moveInputProjectionLen{ moveInputProjection.Length() };
		isInput = moveInputProjectionLen >= MathConstants::EPSILON;
		if (isInput)
		{
			float inputAmount{ std::clamp(inputLen, 0.0f, 1.0f) };
			targetVelocity = moveInputProjection / moveInputProjectionLen
				* _activeRagdoll.settings.maxSpeed
				* inputAmount;
		}
	}

	// BodyStateで計算した現在の水平速度を取得する
	// 目標移動速度と現在速度の差を計算する
	Vector3 delta{ targetVelocity - _activeRagdoll.planarVelocity };

	// 入力がある場合は移動加速度、
	// 入力がない場合は減速度を使用する
	float acceleration{ isInput ? _activeRagdoll.settings.moveAcceleration : _activeRagdoll.settings.moveDeceleration };

	// 制御状態と胴体の傾きから移動操作倍率を計算する
	// 加速度へ移動操作倍率を掛ける
	acceleration *= CalculateMovementControlScale(_activeRagdoll);

	// 1FixedUpdateで変更可能な最大速度差を計算する
	float maxVelocityDelta{ acceleration * TimeManager::GetFixedDeltaTime() };
	// 速度差が最大値を超えていれば長さを制限する
	float deltaLen{ delta.Length() };
	if (deltaLen >= maxVelocityDelta &&
		deltaLen > MathConstants::EPSILON)
	{
		delta *= maxVelocityDelta / deltaLen;
	}

	// 速度差をDeltaTimeで割って必要加速度へ変換する
	delta /= TimeManager::GetFixedDeltaTime();

	// 必要加速度と腰Bodyの質量から移動用Forceを計算する
	Vector3 force{ delta * PhysicsComponentAPI::GetMass(pelvisBodyID) };

	// 腰Bodyへ移動用Forceを加える
	PhysicsComponentAPI::AddForce(pelvisBodyID, force);
}

// 腰と胸へ姿勢を直立させるTorqueを加える関数
void ActiveRagdollSystem::UpdateUpright(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll)
{
	// 制御状態から直立制御の強さを決める
	float strength{ 1.0f };
	// FALLENで無効、AIRBORNEでは弱める
	if (_activeRagdoll.controlState == ActiveRagdollControlState::FALLEN)
	{
		strength = _activeRagdoll.settings.recoveryUprightScale;
	}
	else if (_activeRagdoll.controlState == ActiveRagdollControlState::AIRBORNE)
	{
		strength *= _activeRagdoll.settings.airborneUprightScale;
	}

	// 制御強度がほぼゼロなら終了する
	if (strength <= MathConstants::EPSILON)
	{
		return;
	}

	// Role対応表から腰と胴体のボーンインデックスを取得する
	uint32_t torsoBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::TORSO)] };
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::PELVIS)] };

	// 腰と胴体の各Bodyに対して直立Torqueを計算する
	// Bodyが無効ならそのBodyの処理を飛ばす
	if (torsoBoneIndex == UINT32_MAX || pelvisBoneIndex == UINT32_MAX)
	{
		_activeRagdoll.planarVelocity = Vector3::ZERO;
		_activeRagdoll.uprightDot = -1.0f;
		return;
	}

	// 胴体Body/腰Bodyの現在姿勢を取得する
	BodyID torsoBodyID{ _ragdoll.bodyLinks[torsoBoneIndex].bodyID };
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };

	// 腰/胴体BodyへTorqueを加える
	PhysicsComponentAPI::AddTorque(torsoBodyID, CalculateUprightTorque(_activeRagdoll, torsoBodyID, strength));
	PhysicsComponentAPI::AddTorque(pelvisBodyID, CalculateUprightTorque(_activeRagdoll, pelvisBodyID, strength));
}

// 重心と足の支持範囲から姿勢を安定させる補助Forceを加える関数
void ActiveRagdollSystem::UpdateBalance(const Ragdoll& _ragdoll, ActiveRagdoll& _activeRagdoll)
{
	// STANDINGかつ接地中でなければ終了する
	if (_activeRagdoll.controlState != ActiveRagdollControlState::STANDING ||
		!_activeRagdoll.isGrounded)
	{
		return;
	}

	// Role対応表から腰ボーンインデックスを取得する
	uint32_t pelvisBoneIndex{ _ragdoll.roles[static_cast<size_t>(RagdollBoneRole::PELVIS)] };
	// 腰ボーンインデックスが有効か確認する
	if (pelvisBoneIndex == UINT32_MAX)
	{
		return;
	}
	// 腰ボーンの対応ボディIDを取得
	BodyID pelvisBodyID{ _ragdoll.bodyLinks[pelvisBoneIndex].bodyID };


	// 左右の足から歩行可能な接地点を集める
	bool leftGrounded{ _activeRagdoll.leftFootGround.state == RagdollGroundState::WALKABLE };
	bool rightGrounded{ _activeRagdoll.rightFootGround.state == RagdollGroundState::WALKABLE };
	if (!leftGrounded && !rightGrounded)
	{
		return;
	}

	Vector3 supportStart{ leftGrounded ? _activeRagdoll.leftFootGround.point : _activeRagdoll.rightFootGround.point };
	Vector3 supportEnd{ supportStart };

	// 接地中の足が一つなら、その接地点を支持点にする
	Vector3 targetSupportPoint{ supportStart };
	// 両足が接地している場合は、左右の接地点を結ぶ線分を
	// 簡易的な支持範囲として使用する
	if (leftGrounded && rightGrounded)
	{
		supportStart = _activeRagdoll.leftFootGround.point;
		supportEnd = _activeRagdoll.rightFootGround.point;
	}
	// Ragdollの重心を地面平面へ射影する
	Vector3 groundPlanePoint{ (supportStart + supportEnd) * 0.5f };
	Vector3 projectedCenterOfMass{
		// 地面の位置から重心までのベクトルを地面法線方向をなくすことにより地面平面に射影している
		_activeRagdoll.centerOfMass -
		_activeRagdoll.groundNormal *
		Vector3::Dot(_activeRagdoll.centerOfMass - groundPlanePoint,
			_activeRagdoll.groundNormal)
	};

	// 両足接地時は、射影した重心から支持線分上の
	// 最も近い点を求めて目標支持点にする
	if (leftGrounded && rightGrounded)
	{
		targetSupportPoint = MDMath::ClosestPointOnSegment(projectedCenterOfMass, supportStart, supportEnd);
	}

	// 重心の射影位置から目標支持点までの差を求める
	Vector3 positionError{ targetSupportPoint - projectedCenterOfMass };

	// 差がデッドゾーン内なら位置補正を行わない
	float errorLength{ positionError.Length() };
	float deadZone{ std::max(_activeRagdoll.settings.balanceDeadZone, 0.0f) };
	if (errorLength <= deadZone)
	{
		// 減衰があるから終了はしないよ。
		positionError = Vector3::ZERO;
	}
	else if (errorLength > MathConstants::EPSILON)
	{
		// デッドゾーン分を減らす計算
		positionError *= (errorLength - deadZone) / errorLength;
	}
	// 重心位置の誤差へBalance剛性を掛ける
	Vector3 positionAcceleration{ positionError * _activeRagdoll.settings.balanceStiffness };

	// 現在速度と目標移動速度の差へ減衰を掛ける
	Vector3 targetVelocity{ Vector3::ZERO };
	float inputLength{ _activeRagdoll.moveInput.Length() };
	if (inputLength > MathConstants::EPSILON)
	{
		Vector3 projectedInput{
			_activeRagdoll.moveInput -
			_activeRagdoll.groundNormal *
			Vector3::Dot(
				_activeRagdoll.moveInput,
				_activeRagdoll.groundNormal)
		};

		float projectedInputLength{ projectedInput.Length() };
		if (projectedInputLength > MathConstants::EPSILON)
		{
			float inputAmount{ std::clamp(inputLength, 0.0f, 1.0f) };
			targetVelocity = projectedInput / projectedInputLength * _activeRagdoll.settings.maxSpeed * inputAmount;
		}
	}

	Vector3 velocityAcceleration{
		(targetVelocity - _activeRagdoll.planarVelocity) * _activeRagdoll.settings.balanceDamping
	};

	// 位置補正と速度減衰から必要加速度を計算する
	Vector3 acceleration{ positionAcceleration + velocityAcceleration };

	// 加速度が地面法線方向へ働かないよう平面へ射影する
	acceleration -= _activeRagdoll.groundNormal * Vector3::Dot(acceleration, _activeRagdoll.groundNormal);

	// 必要加速度とRagdoll全体の質量からBalance用Forceを作る
	float totalMass{ 0.0f };
	for (const RagdollBodyLink& bodyLink : _ragdoll.bodyLinks)
	{
		if (bodyLink.bodyID.IsValid())
		{
			totalMass += PhysicsComponentAPI::GetMass(bodyLink.bodyID);
		}
	}

	if (totalMass <= 0.0f)
	{
		return;
	}

	Vector3 force{ acceleration * totalMass };

	// Forceが最大値を超えないよう制限する
	float forceLength{ force.Length() };
	if (forceLength > _activeRagdoll.settings.maxBalanceForce &&
		forceLength > MathConstants::EPSILON)
	{
		force *= _activeRagdoll.settings.maxBalanceForce / forceLength;
	}

	// 腰BodyへBalance用Forceを加える
	PhysicsComponentAPI::AddForce(pelvisBodyID, force);
}

// Animationの目標姿勢から各関節Driveの目標相対回転を更新する関数
void ActiveRagdollSystem::UpdateJointDrive(const Ragdoll& _ragdoll, const ActiveRagdoll& _activeRagdoll, const PoseBuffer& _targetPose)
{
	for (int i{ 0 }; i < _activeRagdoll.jointDriveConstraints.size(); i++)
	{
		/*
			親をオーナーとしている事に注意
		*/
		ConstraintID constraint{ _activeRagdoll.jointDriveConstraints[i] };

		// 親ボーンインデックス
		uint32_t parentBoneIndex{ _activeRagdoll.parentBoneIndex[i] };
		// 子ボーンインデックス
		uint32_t childBoneIndex{ _activeRagdoll.childBoneIndex[i] };

		const EndPointFrame& parentEndPoint{ PhysicsComponentAPI::GetEndPoint(constraint) };
		// 一旦APIの関係上一対一だけどこれからしか取得できないから[0]があってすまぬ
		const EndPointFrame& childEndPoint{ PhysicsComponentAPI::GetOtherEndPoints(constraint)[0] };

		Vector3 pos, scale;
		Quaternion childModelRoatation, parentModelRotation;

		Transform::DecomposeTRS(
			_targetPose.modelFromBoneMatrices[childBoneIndex],
			pos,
			childModelRoatation,
			scale
		);

		Transform::DecomposeTRS(
			_targetPose.modelFromBoneMatrices[parentBoneIndex],
			pos,
			parentModelRotation,
			scale
		);

		// それぞれボディ回転に変換
		Quaternion parentBodyRotation{ parentModelRotation * _ragdoll.bodyLinks[parentBoneIndex].bodyRotationInBoneSpace };
		Quaternion childBodyRotation{ childModelRoatation * _ragdoll.bodyLinks[childBoneIndex].bodyRotationInBoneSpace };

		// 拘束回転に変換(ワールド空間ではないが相対姿勢を求める際には、同じモデル上のため問題ない)
		Quaternion parentJointModel{ parentBodyRotation * parentEndPoint.localRotation };
		Quaternion childJointModel{ childBodyRotation * childEndPoint.localRotation };

		Quaternion targetRelativeRotation{ parentJointModel.Conjugate() * childJointModel };

		targetRelativeRotation.Normalize();

		PhysicsComponentAPI::SetTargetRelativeRotation(constraint, targetRelativeRotation);
	}
}

// 腰の速度から地面法線方向の成分を除いた水平速度を計算する関数
Vector3 ActiveRagdollSystem::CalculatePlanarVelocity(const ActiveRagdoll& _activeRagdoll, BodyID _pelvisBodyID) const
{
	Vector3 velocity{ PhysicsComponentAPI::GetVelocity(_pelvisBodyID) };

	// 接地中なら地面法線を平面の法線として使用する
	// 非接地中ならワールド上方向を使用する(足の時点で計算済みのはず)
	Vector3 groundNormalVelocity{ _activeRagdoll.groundNormal * Vector3::Dot(velocity,_activeRagdoll.groundNormal) };

	// 速度から法線方向の成分を取り除く
	// 地面に沿った速度を返す
	return velocity - groundNormalVelocity;
}

// 各Bodyの位置と質量からRagdoll全体の重心を計算する関数
Vector3 ActiveRagdollSystem::CalculateCenterOfMass(const Ragdoll& _ragdoll) const
{
	Vector3 totalCenterOfMass;
	float totalMass{ 0.0f };
	// 質量付き位置の合計と総質量を初期化する
	for (const RagdollBodyLink& bodyLink : _ragdoll.bodyLinks)
	{
		BodyID bodyID{ bodyLink.bodyID };
		// 無効なBodyは計算対象から除外する
		if (!bodyID.IsValid())
		{
			continue;
		}

		// Bodyの現在位置と質量を取得する
		Vector3 position{ PhysicsComponentAPI::GetRigidBodyPosition(bodyID) };
		float mass{ PhysicsComponentAPI::GetMass(bodyID) };

		// 位置へ質量を掛けて合計へ加算する
		totalCenterOfMass += position * mass;
		// 総質量へBodyの質量を加算する
		totalMass += mass;
	}
	// 総質量がゼロ以下ならゼロベクトルを返す
	if (totalMass <= 0.0f)
	{
		return Vector3::ZERO;
	}

	// 質量付き位置の合計を総質量で割って重心を返す
	return totalCenterOfMass / totalMass;
}

// 胴体の傾きや制御状態から移動操作へ掛ける倍率を計算する関数
float ActiveRagdollSystem::CalculateMovementControlScale(const ActiveRagdoll& _activeRagdoll) const
{
	// 制御状態ごとの基本倍率を決める
	float base{ 0.0f };
	switch (_activeRagdoll.controlState)
	{
	// STANDINGなら通常の操作倍率を使用する
	case ActiveRagdollControlState::STANDING:
		base = 1.0f;
		break;
	// AIRBORNEなら空中操作倍率を使用する
	case ActiveRagdollControlState::AIRBORNE:
		base = _activeRagdoll.settings.airControlScale;
		break;
	// FALLENなら移動操作を無効にする
	case ActiveRagdollControlState::FALLEN:
		return 0.0f;
		break;
	default:
		break;
	}
	// 胴体の直立度を操作可能範囲へ変換する
	float uprightRange{ _activeRagdoll.settings.recoveryUprightDot - _activeRagdoll.settings.fallenUprightDot };

	float uprightScale{ 0.0f };

	if (uprightRange > MathConstants::EPSILON)
	{
		// fallenUprightDot～recoveryUprightDotの範囲で0～1に変換
		// 間は線形補間をする
		uprightScale = std::clamp(
			(_activeRagdoll.uprightDot -
				_activeRagdoll.settings.fallenUprightDot) /
			uprightRange,
			0.0f,
			1.0f
		);
	}

	// 基本倍率へ直立度による倍率を掛ける
	float result{ base * uprightScale };

	// 最終的な操作倍率を0～1へ収めて返す
	return std::clamp(result, 0.0f, 1.0f);
}

// 腰/胸の直立させるためのトルク計算関数
Vector3 ActiveRagdollSystem::CalculateUprightTorque(const ActiveRagdoll& _activeRagdoll, BodyID _bodyID, float _strength) const
{
	// 接地中なら地面法線を目標上方向にする
	// 非接地中ならワールド上方向を使用する(ここまでの過程までに計算済み)
	Vector3 targetUp{ _activeRagdoll.groundNormal };

	// Bodyの現在姿勢からワールド空間の上方向を求める
	Quaternion rotation{ PhysicsComponentAPI::GetRigidBodyRoatation(_bodyID) };

	Vector3 currentUp{ rotation.Rotate(Vector3::UP) };

	// 現在の上方向と目標上方向から
	// 傾きを戻すための回転軸と角度を求める
	Vector3 cross{ Vector3::Cross(currentUp,targetUp) };
	float crossLength{ cross.Length() };
	Vector3 restoringTorque{ Vector3::ZERO };

	// 回転軸と角度へ直立剛性を掛けて復元Torqueを作る
	if (crossLength > MathConstants::EPSILON)
	{
		Vector3 axis{ cross / crossLength };

		float dot{ Vector3::Dot(currentUp, targetUp) };

		float angle{ std::atan2(crossLength, dot) };

		restoringTorque = axis * angle * _activeRagdoll.settings.uprightStiffness;
	}

	// Bodyの角速度を取得する
	Vector3 angularVelocity{ PhysicsComponentAPI::GetAngularVelocity(_bodyID) };

	// 角速度から目標上方向まわりの成分を除き、
	angularVelocity -= targetUp * Vector3::Dot(angularVelocity, targetUp);
	// 傾き方向の回転だけへ減衰を掛ける
	Vector3 dampingTorque{ -angularVelocity * _activeRagdoll.settings.uprightDamping };

	// 復元Torqueと減衰Torqueを合成する
	Vector3 torque = restoringTorque + dampingTorque;

	// 制御状態による倍率を掛ける
	torque *= _strength;

	// Torqueが最大値を超えないよう制限する
	float torqueLen{ torque.Length() };
	if (torqueLen >= _activeRagdoll.settings.maxUprightTorque &&
		torqueLen > MathConstants::EPSILON)
	{
		torque = torque / torqueLen * _activeRagdoll.settings.maxUprightTorque;
	}

	return torque;
}