#include <algorithm>

#include "ColliderTags.h"
#include "PhysicsEvent.h"

#include "CollisionSystem.h"

#include "ContactFunction.h"
#include "GJKEPA.h"

void CollisionSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, EventManager* _eventManager)
{
	// --- 衝突処理 --- 
	BroadPhase(_transformStorage, _colliderStorage);
	Dispatch(_colliderStorage);
	NarrowPhase(_transformStorage, _colliderStorage, _manifoldBuffer, _eventManager);

	// 終了
	End();
}

void CollisionSystem::BroadPhase(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	AABBBroadPhaseColliderStorage* aabbStorage{ _colliderStorage->aabbStorage.get()};
	size_t count = aabbStorage->aabb.size();

	colliderProjectionXValues.reserve(count * 2);
	colliderProjectionYValues.reserve(count * 2);
	colliderProjectionZValues.reserve(count * 2);

	// すべてのコライダーのAABBの各軸の射影を保存する。
	for (int i{ 0 }; i < count; i++)
	{
		uint32_t transformIndex{ _transformStorage->GetDenseIndex(_colliderStorage->aabbStorage->aabb[i].transformID) };

		ColliderProjectionData data;

		data.min = _colliderStorage->aabbStorage->aabb[i].min + _transformStorage->GetPosition(transformIndex);
		data.max = _colliderStorage->aabbStorage->aabb[i].max + _transformStorage->GetPosition(transformIndex);

		// コライダーハンドルのindex
		size_t colliderIndex{ aabbStorage->aabb[i].colliderID.GetIndex()};

		if (projectionDatas.size() > colliderIndex)
		{
			colliderProjectionXValues[projectionDatas[colliderIndex].endpointIndex[ProjectionAxisType::MIN_X]].projection = data.min.x;
			colliderProjectionXValues[projectionDatas[colliderIndex].endpointIndex[ProjectionAxisType::MAX_X]].projection = data.max.x;

			colliderProjectionYValues[projectionDatas[colliderIndex].endpointIndex[ProjectionAxisType::MIN_Y]].projection = data.min.y;
			colliderProjectionYValues[projectionDatas[colliderIndex].endpointIndex[ProjectionAxisType::MAX_Y]].projection = data.max.y;

			colliderProjectionZValues[projectionDatas[colliderIndex].endpointIndex[ProjectionAxisType::MIN_Z]].projection = data.min.z;
			colliderProjectionZValues[projectionDatas[colliderIndex].endpointIndex[ProjectionAxisType::MAX_Z]].projection = data.max.z;
		}
		else
		{
			// 最小値と最大値をそれぞれ追加する。(ColliderProjectionは値/エンティティID/最大値フラグ)
			data.endpointIndex[ProjectionAxisType::MIN_X] = colliderProjectionXValues.size();
			colliderProjectionXValues.push_back(ColliderProjection(data.min.x, aabbStorage->aabb[i].colliderID, false, ProjectionAxisType::MIN_X));
			data.endpointIndex[ProjectionAxisType::MAX_X] = colliderProjectionXValues.size();
			colliderProjectionXValues.push_back(ColliderProjection(data.max.x, aabbStorage->aabb[i].colliderID, true, ProjectionAxisType::MAX_X));

			data.endpointIndex[ProjectionAxisType::MIN_Y] = colliderProjectionYValues.size();
			colliderProjectionYValues.push_back(ColliderProjection(data.min.y, aabbStorage->aabb[i].colliderID, false, ProjectionAxisType::MIN_Y));
			data.endpointIndex[ProjectionAxisType::MAX_Y] = colliderProjectionYValues.size();
			colliderProjectionYValues.push_back(ColliderProjection(data.max.y, aabbStorage->aabb[i].colliderID, true, ProjectionAxisType::MAX_Y));

			data.endpointIndex[ProjectionAxisType::MIN_Z] = colliderProjectionZValues.size();
			colliderProjectionZValues.push_back(ColliderProjection(data.min.z, aabbStorage->aabb[i].colliderID, false, ProjectionAxisType::MIN_Z));
			data.endpointIndex[ProjectionAxisType::MAX_Z] = colliderProjectionZValues.size();
			colliderProjectionZValues.push_back(ColliderProjection(data.max.z, aabbStorage->aabb[i].colliderID, true, ProjectionAxisType::MAX_Z));

			projectionDatas.push_back(data);
		}
	}

	// 射影の値でソートしていく(X,Y,Z全部)
	InsertionSort(colliderProjectionXValues);
	InsertionSort(colliderProjectionYValues);
	InsertionSort(colliderProjectionZValues);
 
	// 各軸で交差しているかの判定を行う
	CheckProjectionAxisValueCross(colliderProjectionXValues);
	CheckProjectionAxisValueCross(colliderProjectionYValues);
	CheckProjectionAxisValueCross(colliderProjectionZValues);

	// 交差マップで3のやつだけナローフェーズに行ける
	for (auto& checkPair : crossCountMap)
	{
		// 3軸すべてで交差しているなら
		if (checkPair.second == 3)
		{
			// ナローフェーズに行けるペアとして追加
			broadClearPairs.push_back(checkPair.first);
		}
	}
}

void CollisionSystem::Dispatch(ColliderStorage* _colliderStorage)
{
	for (auto& pair : broadClearPairs)
	{
		(narrowPhasePairBuilder.*AddPairTable[(int)_colliderStorage->GetType(pair.a)][(int)_colliderStorage->GetType(pair.b)])(pair.a, pair.b);
	}
}

void CollisionSystem::NarrowPhase(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, EventManager* _eventManager)
{
	// 球VS球
	Solve<SphereTag, SphereTag, CollisionPair::SphereSpherePair>
		(
			narrowPhasePairBuilder.sphereSpherePair,
			_transformStorage,
			_colliderStorage,
			_manifoldBuffer,
			_eventManager
		);

	// 箱VS球
	Solve<BoxTag, SphereTag, CollisionPair::BoxSpherePair>
		(
			narrowPhasePairBuilder.boxSpherePair,
			_transformStorage,
			_colliderStorage,
			_manifoldBuffer,
			_eventManager
		);

	// 球VS箱
	Solve<SphereTag, BoxTag, CollisionPair::SphereBoxPair>
		(
			narrowPhasePairBuilder.sphereBoxPair,
			_transformStorage,
			_colliderStorage,
			_manifoldBuffer,
			_eventManager
		);

	// 箱VS箱
	Solve<BoxTag, BoxTag, CollisionPair::BoxBoxPair>
		(
			narrowPhasePairBuilder.boxBoxPair,
			_transformStorage,
			_colliderStorage,
			_manifoldBuffer,
			_eventManager
		);

	// ExitEventの発行
	RegisterExitEvent(_colliderStorage, _eventManager);
}

void CollisionSystem::End()
{
	// リセット
	crossCountMap.clear();
	broadClearPairs.clear();

	narrowPhasePairBuilder.Clear();

	// 前回フレーム衝突に追加
	for (auto& pair : currentFramePair)
	{
		if (!prevFramePair.contains(pair))
		{
			prevFramePair.insert(pair);
		}
	}

	// 追加後にリセット
	currentFramePair.clear();
}

void CollisionSystem::CheckProjectionAxisValueCross(std::vector<ColliderProjection>& _projectionAxisValues)
{
	// 値がminだった時ここに追加してmaxが来たら消す(つまりminの値しか入らない)
	std::vector<size_t> actives;

	// 軸の判定
	for (int i{ 0 }; i < _projectionAxisValues.size(); i++)
	{
		// 最小値
		if (_projectionAxisValues[i].isMax == false)
		{
			// まだ、maxが来てないやつら全員と比較
			for (size_t active : actives)
			{
				// 同ペア対策
				if (_projectionAxisValues[i].colliderID.GetIndex() > _projectionAxisValues[active].colliderID.GetIndex())
				{
					// 軸で交差しているので交差カウント増加
					crossCountMap[CollisionPair::Pair(_projectionAxisValues[i].colliderID, _projectionAxisValues[active].colliderID)] += 1;
				}
				else
				{
					// 軸で交差しているので交差カウント増加
					crossCountMap[CollisionPair::Pair(_projectionAxisValues[active].colliderID, _projectionAxisValues[i].colliderID)] += 1;
				}
			}
			// 最小値なのでactiveリストに追加。
			_projectionAxisValues[i].activeIndex = actives.size();
			actives.push_back(i);
		}
		// 最大値
		else
		{
			// activeリストから消す
			ProjectionAxisType minAxis{ static_cast<ProjectionAxisType>(_projectionAxisValues[i].axisType - 1) };
			// プロジェクションデータとColliderIDのIndexは紐づいているためこれで取得できる
			uint32_t minAxisIndex{ projectionDatas[_projectionAxisValues[i].colliderID.GetIndex()].endpointIndex[minAxis]};

			// 移動した奴のIndexの更新
			_projectionAxisValues[actives.back()].activeIndex = _projectionAxisValues[minAxisIndex].activeIndex;

			actives[_projectionAxisValues[minAxisIndex].activeIndex] = actives.back();
			actives.pop_back();
		}
	}
}

template<class A, class B, class PairList>
void CollisionSystem::Solve(
	const std::vector<PairList>& pairList,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer,
	EventManager* _eventManager)
{
	// ナローフェーズに行けたペアの衝突判定をしていく
	for (auto& pair : pairList)
	{
		if (GJKEPA::GJK<A, B>(pair.a, pair.b, _colliderStorage, _transformStorage, _manifoldBuffer))
		{
			RegisterEvent(pair.a, pair.b, _colliderStorage, _eventManager);
		}
	}
}

template<>
void CollisionSystem::Solve<BoxTag, BoxTag, CollisionPair::BoxBoxPair>(
	const std::vector<CollisionPair::BoxBoxPair>& pairList,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer,
	EventManager* _eventManager)
{
	for (auto& pair : pairList)
	{
		if (ContactFunction::BoxBox(pair.a, pair.b, _colliderStorage, _transformStorage, _manifoldBuffer))
		{
			RegisterEvent(pair.a, pair.b, _colliderStorage, _eventManager);
		}
	}
}

template<>
void CollisionSystem::Solve<SphereTag, SphereTag, CollisionPair::SphereSpherePair>(
	const std::vector<CollisionPair::SphereSpherePair>& pairList,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer,
	EventManager* _eventManager)
{
	for (auto& pair : pairList)
	{
		if (ContactFunction::SphereSphere(pair.a, pair.b, _colliderStorage, _transformStorage, _manifoldBuffer))
		{
			RegisterEvent(pair.a, pair.b, _colliderStorage, _eventManager);
		}
	}
}

template<>
void CollisionSystem::Solve<SphereTag, BoxTag, CollisionPair::SphereBoxPair>(
	const std::vector<CollisionPair::SphereBoxPair>& pairList,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer,
	EventManager* _eventManager)
{
	for (auto& pair : pairList)
	{
		if (ContactFunction::SphereBox(pair.a, pair.b, _colliderStorage, _transformStorage, _manifoldBuffer))
		{
			RegisterEvent(pair.a, pair.b, _colliderStorage, _eventManager);
		}
	}
}
template<>
void CollisionSystem::Solve<BoxTag, SphereTag, CollisionPair::BoxSpherePair>(
	const std::vector<CollisionPair::BoxSpherePair>& pairList,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer,
	EventManager* _eventManager)
{
	for (auto& pair : pairList)
	{
		if (ContactFunction::SphereBox(pair.b, pair.a, _colliderStorage, _transformStorage, _manifoldBuffer))
		{
			RegisterEvent(pair.a, pair.b, _colliderStorage, _eventManager);
		}
	}
}

void CollisionSystem::RegisterEvent(
	ColliderID _a,
	ColliderID _b,
	ColliderStorage* _colliderStorage,
	EventManager* _eventManager)
{
	// 今回のペア追加
	currentFramePair.insert(CollisionPair::Pair{ _a, _b });

	// 当たっているのでとりあえずよべる
	_eventManager->Push<OnCollisionEvent>({ _colliderStorage->GetOwnerEntity(_a),_colliderStorage->GetOwnerEntity(_b) });

	// 前回のフレーム当たってなくて今回当たってるため衝突開始のイベントを呼ぶ
	if (!prevFramePair.contains(CollisionPair::Pair{ _a, _b }))
	{
		_eventManager->Push<OnCollisionEnterEvent>({ _colliderStorage->GetOwnerEntity(_a),_colliderStorage->GetOwnerEntity(_b) });
	}
}

void CollisionSystem::RegisterExitEvent(ColliderStorage* _colliderStorage, EventManager* _eventManager)
{
	std::unordered_set<CollisionPair::Pair> erasePair;

	// OnCollisionExit呼び出し
	for (auto& pair : prevFramePair)
	{
		// 前フレーム当たってて今回の衝突ペアにいないから衝突しなくなった
		if (!currentFramePair.contains(pair))
		{
			// イベント呼び出し
			_eventManager->Push<OnCollisionExitEvent>({ _colliderStorage->GetOwnerEntity(pair.a),_colliderStorage->GetOwnerEntity(pair.b) });

			erasePair.insert(pair);
		}
	}

	// 削除
	for (auto& pair : erasePair)
	{
		prevFramePair.erase(pair);
	}
}

void CollisionSystem::InsertionSort(std::vector<ColliderProjection>& _projectionValues)
{
	// ProjectionDataのストレージの変更も入れる事。
	size_t size{ _projectionValues.size() };
	for (int i{ 1 }; i < size; ++i)
	{
		int j{ i };

		while (j > 0 && _projectionValues[j - 1].projection > _projectionValues[j].projection)
		{
			// 入れ替え
			Swap(_projectionValues, j - 1, j);

			j--;
		}
	}
}

void CollisionSystem::Swap(std::vector<ColliderProjection>& _projectionValues, int _a, int _b)
{
	// 元の要素を入れ替えてその後の値を見て値を変える。
	std::swap(_projectionValues[_a], _projectionValues[_b]);

	projectionDatas[_projectionValues[_a].colliderID.GetIndex()].endpointIndex[_projectionValues[_a].axisType] = _a;
	projectionDatas[_projectionValues[_b].colliderID.GetIndex()].endpointIndex[_projectionValues[_b].axisType] = _b;
}
