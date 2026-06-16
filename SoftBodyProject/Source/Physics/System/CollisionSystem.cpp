#include <algorithm>

#include "TransformComponent.h"
#include "ColliderComponentStorage.h"

#include "CollisionSystem.h"

void CollisionSystem::FixedUpdate(IWorld* world)
{
	// コライダーストレージ
	SparseSetStorageBase<ColliderComponent>* colliderStorage{ world->GetStorage<ColliderComponent>() };
	// Transformストレージ
	SparseSetStorageBase<TransformComponent>* transformStorage{ world->GetStorage<TransformComponent>() };
}

void CollisionSystem::BroadPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage)
{
	/*
		後から、インサートソートに変更するがGJKアルゴリズムが動くまで(すべての形)は追加ー＞ソートで対応。
	*/
	// 参照用
	TransformComponent trans{};

	// すべてのコライダーのAABBの各軸の射影を保存する。
	for (int entity : *colliderStorage->GetEntities())
	{
		// Transformがあるかチェックないなら飛ばす
		if (!transformStorage->TryGet(entity, trans))
		{
			continue;
		}

		// 各軸に射影して値を保存
		ColliderComponent* col{ colliderStorage->Get(entity) };

		const Vector3& min{ col->GetBroadMin() + trans.GetPosition() };
		const Vector3& max{ col->GetBroadMax() + trans.GetPosition() };

		// 最小値と最大値をそれぞれ追加する。(ColliderProjectionは値/エンティティID/最大値フラグ)
		colliderProjectionXValues.push_back(ColliderProjection(min.x, entity, false));
		colliderProjectionXValues.push_back(ColliderProjection(max.x, entity, true));

		colliderProjectionYValues.push_back(ColliderProjection(min.y, entity, false));
		colliderProjectionYValues.push_back(ColliderProjection(max.y, entity, true));

		colliderProjectionZValues.push_back(ColliderProjection(min.z, entity, false));
		colliderProjectionZValues.push_back(ColliderProjection(max.z, entity, true));
	}

	// 射影の値でソートしていく(X,Y,Z全部)
	std::sort(colliderProjectionXValues.begin(), colliderProjectionXValues.end(),
		[](const ColliderProjection& a, const ColliderProjection& b)
		{
			// 値が等しくないなら比較
			if (a.projection != b.projection)
			{
				return a.projection < b.projection;
			}
			// 同じならMinを先に置きたいのでtrueが後になるように
			return a.isMax < b.isMax;
		});

	std::sort(colliderProjectionYValues.begin(), colliderProjectionYValues.end(),
		[](const ColliderProjection& a, const ColliderProjection& b)
		{
			// 値が等しくないなら比較
			if (a.projection != b.projection)
			{
				return a.projection < b.projection;
			}
			// 同じならMinを先に置きたいのでtrueが後になるように
			return a.isMax < b.isMax;
		});

	std::sort(colliderProjectionZValues.begin(), colliderProjectionZValues.end(),
		[](const ColliderProjection& a, const ColliderProjection& b)
		{
			// 値が等しくないなら比較
			if (a.projection != b.projection)
			{
				return a.projection < b.projection;
			}
			// 同じならMinを先に置きたいのでtrueが後になるように
			return a.isMax < b.isMax;
		});
 
	// 各軸で交差しているかの判定を行う
	CheckProjectionAxisValueCross(colliderProjectionXValues);
	CheckProjectionAxisValueCross(colliderProjectionYValues);
	CheckProjectionAxisValueCross(colliderProjectionZValues);

	// 交差マップで3のやつだけナローフェーズに行ける
	for (auto checkPair : crossCountMap)
	{
		// 3軸すべてで交差しているなら
		if (checkPair.second == 3)
		{
			// ナローフェーズに行けるペアとして追加
			narrowPairs.push_back(checkPair.first);
		}
	}
}

void CollisionSystem::NarrowPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage)
{

}

void CollisionSystem::CheckProjectionAxisValueCross(const std::vector<ColliderProjection>& projectionAxisValues)
{
	// 値がminだった時ここに追加してmaxが来たら消す(つまりminの値しか入らない)
	std::vector<int> actives;

	// 軸の判定
	for (auto& check : projectionAxisValues)
	{
		// 最小値
		if (check.isMax == false)
		{
			// まだ、maxが来てないやつら全員と比較
			for (int active : actives)
			{
				// 軸で交差しているので交差カウント増加
				crossCountMap[
					CollPair(
						// 同ペア対策
						std::min(check.entity, active),
						std::max(check.entity, active))]
					+= 1;
			}

			// 最小値なのでactiveリストに追加。
			actives.push_back(check.entity);
		}
		// 最大値
		else
		{
			/*
				削除時にfind関数が残っているためColliderProjectionにactivesのIndexを持たせて削除させてもよい
			*/
			// activeリストから消す
			auto it = std::find(
				actives.begin(),
				actives.end(),
				check.entity);

			// 一応チェックしてから
			if (it != actives.end())
			{
				// 削除
				*it = actives.back();
				actives.pop_back();
			}
		}
	}
}

bool CollisionSystem::GJK(
	const ColliderComponent& collider01, const TransformComponent& transform01,
	const ColliderComponent& collider02, const TransformComponent& transform02)
{
	/*
		コライダー01の中心を原点として考えるものとしよう。
	*/
	// 上方向の支点を最初の支点とする
	Vector3 dir{ Vector3::UP };
	// ミンコフスキー差の支点(上方向)
	Vector3 fulcrums[3]
	{ 
		// 初期の点以外はゼロで初期化
		collider01.Support(dir) - collider02.Support(-dir) + (transform02.GetPosition() - transform01.GetPosition())
		,Vector3::ZERO,Vector3::ZERO
	};
	// 線分,原点に最も近い点(While内で使うが毎回生成するのはめんどいのでここで作っておく)
	Vector3 segment,originNearestPoint;

	// 上の支点を最初の点として方向を生成
	dir = -fulcrums[0];
	// 生成した方向で第二の支点を計算
	fulcrums[1] = collider01.Support(dir) - collider02.Support(-dir) + (transform02.GetPosition() - transform01.GetPosition());
	// 判定用の内積(上で生成された支点と方向の内積を入れている)
	float dot{ Vector3::Dot(fulcrums[1],dir) };

	// どのインデックスに点を入れるのか(今、0,1には初期点とその点から生成した方向とその時の支点が入っているので最後の2の要素がスタート)
	char index{ 2 };
	// 新しく出した支点とその時の方向の内積が0以下なら原点にこれ以上向かう事が出来ないとして終了
	while (dot <= 0.0f)
	{
		// 点の生成
		// 既存の2つの点から線分を生成(インデックスは決めるインデックスの次とその次)
		char startIndex{ (index + 1) % 3 };
		//        スタート地点           終点地点
		segment = fulcrums[startIndex] - fulcrums[(index + 2) % 3];
		// 線と原点の最近点を計算
		originNearestPoint = fulcrums[startIndex] + segment * Vector3::Dot(segment.Normalize(), Vector3::ZERO);
		// 方向計算
		dir = -originNearestPoint;
		// この新しい方向から新たな点を計算
		fulcrums[index] = collider01.Support(dir) - collider02.Support(-dir) + (transform02.GetPosition() - transform01.GetPosition());

		// 4面体内に原点が含まれているか判定

	}

	return false;
}