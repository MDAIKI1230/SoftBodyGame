#pragma once

/*
	ラグドール生成時のコライダーの情報
	形状の情報は持ってないので他で持つ必要がある
*/
union RagdollColliderDefinition
{
	struct Sphere { float radius{ 0.0f }; };
	struct Capsule { float height{ 0.0f }, radius{ 0.0f }; };
	struct Box { float width{ 0.0f }, height{ 0.0f }, depth{ 0.0f }; };

	Sphere sphere;
	Capsule capsule;
	Box box;
	// デフォルトコンストラクタ(カプセルの場合が多いと思うからカプセル)
	RagdollColliderDefinition()
		: capsule{}
	{
	}
};