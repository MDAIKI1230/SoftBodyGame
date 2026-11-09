#pragma once

#include "LightConstants.h"

#include "LightHandle.h"

#include "Color.h"
#include "MDMath.h"

struct LightComponent
{
	friend class LightSystem;
public:
	LightComponent();

	// コピーは禁止
	LightComponent(const LightComponent&) = delete;
	LightComponent& operator=(const LightComponent&) = delete;

	// ムーブで所有権を渡す
	LightComponent(LightComponent&& _other) noexcept;
	LightComponent& operator=(LightComponent&& _other) noexcept;
	~LightComponent();

	// ライトの種類の取得
	LightType GetType() const;
	// ライトの種類の設定
	void SetType(LightType _type);

	// ライトのディフューズカラーの取得
	const Color& GetDiffuseColor() const;
	// ライトのディフューズカラーの設定
	void SetDiffuseColor(const Color& _color);

	// ライトのアンビエントカラーの取得
	const Color& GetAmbientColor() const;
	// ライトのアンビエントカラーの設定
	void SetAmbientColor(const Color& _color);

	// ライトの有効フラグの取得
	bool GetEnable() const;
	// ライトの有効フラグの設定
	void SetEnable(bool _enable);

	// 光の強度の取得
	float GetIntensity() const;
	// 光の強度の設定
	void SetIntensity(float _intensity);

	// 光が届く距離の取得
	float GetRange() const;
	// 光が届く距離の設定
	void SetRange(float _range);

	// スポットライトの減衰が始まる角度の取得
	float GetInnerAngle() const;
	// スポットライトの減衰が始まる角度の設定
	void SetInnerAngle(float _innerAngle);

	// スポットライトの光が届く外側の角度の取得
	float GetOuterAngle() const;
	// スポットライトの光が届く外側の角度の設定
	void SetOuterAngle(float _outerAngle);
private:
	void SetWorldPose(const Vector3& _position, const Vector3& _direction);
	bool EnsureLight();
	void ApplyDiffuseColor();

private:
	// ハンドル
	LightHandle handle;
	// ライトの種類
	LightType type{ LightType::DIRECTIONAL };
	// ディフューズカラー
	Color diffuseColor{ 1.0f, 1.0f, 1.0f };
	// アンビエントカラー
	Color ambientColor{ 0.0f, 0.0f, 0.0f };
	// 有効化フラグ
	bool enable{ true };
	// 光強度
	float intensity{ 1.0f };
	// 届く範囲
	float range{ 0.0f };
	// 減衰が始まる角度
	float innerAngle{ 0.0f };
	// 光が届く外側の角度
	float outerAngle{ 0.0f };
};