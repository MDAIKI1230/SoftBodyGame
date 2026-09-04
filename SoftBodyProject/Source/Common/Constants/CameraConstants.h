#pragma once

enum class ProjectionType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

enum class ClearMode :uint32_t
{
	SOLID_COLOR,
	SKY
};

enum class CameraMode
{
	FPS,
	TPS
};
