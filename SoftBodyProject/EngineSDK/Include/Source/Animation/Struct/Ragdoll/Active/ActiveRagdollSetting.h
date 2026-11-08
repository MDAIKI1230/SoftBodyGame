#pragma once

/*
	アクティブラグドールの設定構造体
*/
struct ActiveRagdollSetting
{
	// --- 起き上がり制御 ---

	// 腰Bodyの位置から地面探索レイを開始する際の上方向オフセット
	float recoveryProbeStartOffset{};
	// 目標腰高に追加する地面探索距離
	float recoveryProbeDistance{};
	// 地面法線と上方向の内積がこの値以上なら起き上がりの補助力を加える
	float recoveryMinGroundDot{};
	// 地面から維持したい腰までの高さ
	float recoveryTargetPelvisHeight{};
	// 腰高の誤差を持ち上げるForceへ変換する比例係数
	float recoveryStiffness{};
	// 腰の上下動を抑える減衰係数
	float recoveryDamping{};
	// 起き上がりの補助力の上限
	float recoveryMaxForce{};

	// --- 姿勢制御(BoneIndexに合わせる) ---

	// Animationの目標相対姿勢へ戻す関節駆動拘束のコンプライアンス値
	std::vector<float> rotationConstraintCompliances;
	// 位置拘束が一つの回転軸へ出せる最大強度
	std::vector<float> roatationConstraintMaxPowers;
	// Animationの目標相対姿勢へ戻す関節駆動拘束のコンプライアンス値
	std::vector<float> positionConstraintCompliances;
	// 位置拘束が一つの回転軸へ出せる最大強度
	std::vector<float> positionConstraintMaxPowers;

	// 空中にいるときの倍率
	std::vector<float> airborneFollowScales;
};