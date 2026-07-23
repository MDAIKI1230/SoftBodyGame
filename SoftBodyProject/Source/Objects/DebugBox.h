#pragma once

#include "ObjectBase.h"
#include "WorldStorage.h"

class DebugBox :public ObjectBase
{
public:
	// コンストラクタ
	DebugBox(WorldStorage* _world, const EntityID& _entity);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_size">一辺の大きさ</param>
	DebugBox(WorldStorage* _world, const EntityID& _entity, float _size);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_width">X方向の長さ</param>
	/// <param name="_height">Y方向の長さ</param>
	/// <param name="_depth">Z方向の長さ</param>
	DebugBox(WorldStorage* _world, const EntityID& _entity, float _width, float _height, float _depth);
	// --- 更新系 ---

	void Update() override;
	void FixedUpdate() override;

	// --- 衝突系 ---

	void OnCollisionEnter() override;
	void OnCollision() override;
	void OnCollisionExit() override;
};
#pragma once
