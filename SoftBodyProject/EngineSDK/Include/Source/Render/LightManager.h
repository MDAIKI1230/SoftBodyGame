#pragma once

#include "Color.h"
#include "LightConstants.h"
#include "LightHandle.h"
#include "MDMath.h"

class LightManager
{
	friend struct LightComponent;
	friend struct LightComponentDocument;

private:
	LightManager() = delete;

	static LightHandle Create();
	static void ChangeType(LightHandle _handle, LightType _type);
	static void SetEnable(LightHandle _handle, bool _flag);
	static void SetDirection(LightHandle _handle, const Vector3& _dir);
	static void SetPosition(LightHandle _handle, const Vector3& _pos);
	static void SetRangeAngle(LightHandle _handle, float _range, float _innerAngle, float _outerAngle);
	static void SetDiffuseColor(LightHandle _handle, const Color& _color);
	static void SetAmbientColor(LightHandle _handle, const Color& _color);
	static void Delete(LightHandle _handle);
};