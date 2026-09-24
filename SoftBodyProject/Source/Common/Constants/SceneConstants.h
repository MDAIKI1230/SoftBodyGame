#pragma once

enum class SceneState
{
	INITIALIZE,
	LOADING,
	FADEIN,
	UPDATE,
	TERMINATE,
	FADEOUT
};

enum class SceneRequestType
{
	NONE,
	TITLE,
	GAME
};