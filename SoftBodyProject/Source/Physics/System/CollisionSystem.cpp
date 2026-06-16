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