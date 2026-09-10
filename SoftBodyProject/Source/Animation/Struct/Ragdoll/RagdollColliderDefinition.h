#pragma once

/*
	ラグドール生成時のコライダーの情報
	形状の情報は持ってないので他で持つ必要がある
*/
union RagdollColliderDefinition
{
	struct Sphere { float radius; };
	struct Capsule { float height, radius; };
	struct Box { float width, height, depth; };

	Sphere sphere;
	Capsule capsule;
	Box box;
};