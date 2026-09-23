#pragma once

/*
	アクティブラグドールの設定構造体
*/
struct ActiveRagdollSetting
{
	// --- 接地判定 ---

	// 足Bodyの位置から地面探索レイを開始する際の上方向オフセット
	float groundProbeStartOffset{};
	// 足から地面を探索するレイの最大距離
	float groundProbeDistance{};
	// 地面法線と上方向の内積がこの値以上なら歩行可能と判定する
	float minGroundDot{};


	// --- 移動制御 ---

	// 移動入力による最大水平速度
	float maxSpeed{};
	// 移動入力がある場合に目標速度へ近づける最大加速度
	float moveAcceleration{};
	// 移動入力がない場合に水平速度を減速させる最大加速度
	float moveDeceleration{};
	// 空中で移動操作へ掛ける倍率
	// 0で空中操作なし、1で地上と同じ操作強度
	float airControlScale{};


	// --- 制御状態判定 ---

	// 接地を失ってからAIRBORNEへ遷移するまでの猶予時間
	float airborneDelay{};
	// 胴体の直立度がこの値以下なら転倒判定を開始する
	float fallenUprightDot{};
	// 転倒姿勢が継続してからFALLENへ遷移するまでの時間
	float fallDelay{};
	// 胴体の直立度がこの値以上なら立位への復帰判定を開始する
	// 状態の往復を防ぐためfallenUprightDotより大きい値にする
	float recoveryUprightDot{};
	// 復帰可能な姿勢が継続してからSTANDINGへ遷移するまでの時間
	float recoveryDelay{};


	// --- 直立制御 ---

	// 腰と胴体の傾きを目標上方向へ戻す復元Torqueの比例係数
	float uprightStiffness{};
	// 腰と胴体の傾き方向の角速度を抑える減衰係数
	float uprightDamping{};
	// 一つのBodyへ加える直立制御Torqueの最大値
	float maxUprightTorque{};
	// 空中で直立制御Torqueへ掛ける倍率
	// 0で空中制御なし、1で地上と同じ制御強度
	float airborneUprightScale{};


	// --- バランス制御 ---

	// 重心と足の支持範囲との位置誤差へ掛ける比例係数
	float balanceStiffness{};
	// 現在速度と目標移動速度との差へ掛ける減衰係数
	float balanceDamping{};
	// 腰Bodyへ加えるバランス制御Forceの最大値
	float maxBalanceForce{};
	// 重心が支持位置からこの距離以内にある場合は位置補正を行わない
	float balanceDeadZone{};


	// --- 関節駆動制御 ---

	// Animationの目標相対姿勢へ戻す関節駆動拘束の比例係数
	float jointDriveStiffness{};
	// 関節駆動による回転の振動を抑える減衰係数
	float jointDriveDamping{};
	// 関節駆動拘束が一つの回転軸へ出せる最大強度
	float maxJointDriveForce{};

	// --- ジャンプ制御 ---

	// 到達させたいジャンプの高さ
	float jumpHeight{};
	// ジャンプ直後に接地判定による着地遷移を禁止する時間
	float jumpGroundIgnoreTime{};


	// --- 立ち上がり制御 ---

	// 体を上下へ伸ばす立ち上がりForceの重力比
	float recoveryLiftScale{};
	// 転倒中の直立制御Torqueへ掛ける最大倍率
	float recoveryUprightScale{};

	// --- 腰高さ制御 ---
	// 地面から維持したい腰までの高さ
	float targetPelvisHeight{};
	// 腰高の誤差を持ち上げるForceへ変換する比例係数
	float pelvisHeightStiffness{};
	// 腰の上下動を抑える減衰係数
	float pelvisHeightDamping{};
	// 腰高制御が出せる最大Force
	float maxPelvisLiftForce{};
};