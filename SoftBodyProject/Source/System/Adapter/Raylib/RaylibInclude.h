#pragma once

#define Vector2    RlVector2
#define Vector3    RlVector3
#define Vector4    RlVector4
#define Quaternion RlQuaternion
#define Matrix     RlMatrix
#define Color      RlColor
#define Camera     RlCamera
#define Camera2D   RlCamera2D
#define Camera3D   RlCamera3D
#define Transform  RlTransform

using RlModel = ::Model;
using RlTexture2D = ::Texture2D;

#include <raylib.h>
#include <rlgl.h>

#undef Vector2
#undef Vector3
#undef Vector4
#undef Quaternion
#undef Matrix
#undef Color
#undef Camera
#undef Camera2D
#undef Camera3D
#undef Transform
