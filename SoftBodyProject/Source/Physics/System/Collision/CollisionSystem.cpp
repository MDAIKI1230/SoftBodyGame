#include <algorithm>

#include "ColliderTags.h"

#include "CollisionSystem.h"

void CollisionSystem::FixedUpdate(WorldStorage* _world)
{
	// コライダーストレージ
	SphereColliderComponentStorage* sphereStorage{ static_cast<SphereColliderComponentStorage*>(_world->GetStorage<SphereColliderComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage { static_cast<TransformComponentStorage*>(_world->GetStorage<TransformComponent>()) };
	// オブジェクトマネージャー
	ObjectManager* objectManager{  };

	// --- 衝突処理 --- 
	BroadPhase(transformStorage, sphereStorage);
	NarrowPhase(transformStorage, sphereStorage, objectManager);

	// 終了
	End();
}

void CollisionSystem::BroadPhase(TransformComponentStorage* _transformStorage, SphereColliderComponentStorage* _sphereStorage)
{
	size_t count = _sphereStorage->GetSize();

	colliderProjectionXValues.reserve(count * 2);
	colliderProjectionYValues.reserve(count * 2);
	colliderProjectionZValues.reserve(count * 2);

	// 参照用
	TransformComponent trans{};

	// すべてのコライダーのAABBの各軸の射影を保存する。
	for (int entity : *_sphereStorage->GetEntities())
	{
		// Transformがあるかチェックないなら飛ばす
		if (!_transformStorage->TryGet(entity, trans))
		{
			continue;
		}

		// 各軸に射影して値を保存
		ColliderComponent* col{ _sphereStorage->Get(entity) };

		ColliderProjectionData data;

		data.min = col->GetBroadMin() + trans.GetPosition();
		data.max = col->GetBroadMax() + trans.GetPosition();

		if (projectionStorage.TryGet(entity,data))
		{
			colliderProjectionXValues[data.endpointIndex[ProjectionAxisType::MIN_X]].projection = data.min.x;
			colliderProjectionXValues[data.endpointIndex[ProjectionAxisType::MAX_X]].projection = data.max.x;

			colliderProjectionYValues[data.endpointIndex[ProjectionAxisType::MIN_Y]].projection = data.min.y;
			colliderProjectionYValues[data.endpointIndex[ProjectionAxisType::MAX_Y]].projection = data.max.y;

			colliderProjectionZValues[data.endpointIndex[ProjectionAxisType::MIN_Z]].projection = data.min.z;
			colliderProjectionZValues[data.endpointIndex[ProjectionAxisType::MAX_Z]].projection = data.max.z;
		}
		else
		{
			// 最小値と最大値をそれぞれ追加する。(ColliderProjectionは値/エンティティID/最大値フラグ)
			data.endpointIndex[ProjectionAxisType::MIN_X] = colliderProjectionXValues.size();
			colliderProjectionXValues.push_back(ColliderProjection(data.min.x, entity, false, ProjectionAxisType::MIN_X));
			data.endpointIndex[ProjectionAxisType::MAX_X] = colliderProjectionXValues.size();
			colliderProjectionXValues.push_back(ColliderProjection(data.max.x, entity, true, ProjectionAxisType::MAX_X));

			data.endpointIndex[ProjectionAxisType::MIN_Y] = colliderProjectionYValues.size();
			colliderProjectionYValues.push_back(ColliderProjection(data.min.y, entity, false, ProjectionAxisType::MIN_Y));
			data.endpointIndex[ProjectionAxisType::MAX_Y] = colliderProjectionYValues.size();
			colliderProjectionYValues.push_back(ColliderProjection(data.max.y, entity, true, ProjectionAxisType::MAX_Y));

			data.endpointIndex[ProjectionAxisType::MIN_Z] = colliderProjectionZValues.size();
			colliderProjectionZValues.push_back(ColliderProjection(data.min.z, entity, false, ProjectionAxisType::MIN_Z));
			data.endpointIndex[ProjectionAxisType::MAX_Z] = colliderProjectionZValues.size();
			colliderProjectionZValues.push_back(ColliderProjection(data.max.z, entity, true, ProjectionAxisType::MAX_Z));

			projectionStorage.Add(entity, std::move(data));
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

void CollisionSystem::NarrowPhase(TransformComponentStorage* _transformStorage, SphereColliderComponentStorage* _sphereStorage, ObjectManager* _objectManager)
{
	// 参照用
	TransformComponent transA{}, transB{};

	// ナローフェーズに行けたペアの衝突判定をしていく
	for (auto& pair : narrowPairs)
	{
		// Transformがあるかチェックないなら飛ばす
		if (!_transformStorage->TryGet(pair.a, transA))
		{
			continue;
		}
		if (!_transformStorage->TryGet(pair.b, transB))
		{
			continue;
		}

		// 各コライダー取得
		ColliderComponent* collA{ _sphereStorage->Get(pair.a) };
		ColliderComponent* collB{ _sphereStorage->Get(pair.b) };

		if (Solve(_sphereStorage, _sphereStorage, pair, _transformStorage))
		{
			// 今回のペア追加
			currentFramePair.insert(pair);

			ObjectBase* objA{ _objectManager->Get(pair.a) };
			ObjectBase* objB{ _objectManager->Get(pair.b) };
			// 当たっているのでとりあえずよべる
			if (objA != nullptr)
			{
				objA->OnCollision();
			}
			if (objB != nullptr)
			{
				objB->OnCollision();
			}

			// 前回のフレーム当たってなくて今回当たってるため衝突開始のイベントを呼ぶ
			if (!prevFramePair.contains(pair))
			{
				if (objA != nullptr)
				{
					objA->OnCollisionEnter();
				}

				if (objB != nullptr)
				{
					objB->OnCollisionEnter();
				}
			}
		}
	}

	std::unordered_set<SphereSpherePair> erasePair;

	// OnCollisionExit呼び出し
	for (auto& pair : prevFramePair)
	{
		// 前フレーム当たってて今回の衝突ペアにいないから衝突しなくなった
		if (!currentFramePair.contains(pair))
		{
			// 取得
			ObjectBase* objA{ _objectManager->Get(pair.a) };
			ObjectBase* objB{ _objectManager->Get(pair.b) };
			// イベント呼び出し
			if (objA != nullptr)
			{
				objA->OnCollisionExit();
			}
			if (objB != nullptr)
			{
				objB->OnCollisionExit();
			}

			erasePair.insert(pair);
		}
	}

	// 削除
	for (auto& pair : erasePair)
	{
		prevFramePair.erase(pair);
	}
}

void CollisionSystem::End()
{
	// 全リセット
	crossCountMap.clear();
	narrowPairs.clear();

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

void CollisionSystem::CheckProjectionAxisValueCross(const std::vector<ColliderProjection>& _projectionAxisValues)
{
	// 値がminだった時ここに追加してmaxが来たら消す(つまりminの値しか入らない)
	std::vector<int> actives;

	// 軸の判定
	for (auto& check : _projectionAxisValues)
	{
		// 最小値
		if (check.isMax == false)
		{
			// まだ、maxが来てないやつら全員と比較
			for (int active : actives)
			{
				// 軸で交差しているので交差カウント増加
				crossCountMap[
					SphereSpherePair(
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

template<class A, class B, class AS, class BS>
bool CollisionSystem::GJK(
	AS* _storageA, int _handleA,
	BS* _storageB, int _handleB,
	TransformComponentStorage* _transformStorage)
{
	/*
		コライダー01の中心を原点として考えるものとしよう。
	*/

	Simplex simplex{};
	Vector3 dir{ Vector3::UP };

	// トランスフォーム取得
	TransformComponent* transformA{ _transformStorage->Get(_handleA) };
	TransformComponent* transformB{ _transformStorage->Get(_handleB) };

	while (true)
	{
		// ミンコフスキー差の支点計算
		Vector3 vec{ A::Support(_storageA,_handleA,dir) - B::Support(_storageB,_handleB,-dir) + (transformB->GetPosition() - transformA->GetPosition()) };

		// 支点をSimplexに追加
		simplex.Add(vec);

		// 内積から当たる可能性があるのか見てみる
		if (Vector3::Dot(vec, dir) < 0)
		{
			return false;
		}

		if(SimplexSolve(simplex,dir))
		{
			return true;
		}
	}

	return false;
}

bool CollisionSystem::Solve(SphereColliderComponentStorage* _strageA, SphereColliderComponentStorage* _strageB, SphereSpherePair& pair,TransformComponentStorage* _transformStorage)
{
	return GJK<ColliderTag::SphereTag, ColliderTag::SphereTag>(_strageA, pair.a, _strageB, pair.b, _transformStorage);
}

bool CollisionSystem::SimplexSolve(Simplex& _simplex, Vector3& _output)
{
	switch (_simplex.GetSize())
	{
	// 点
	case 1: return SolvePoint(_simplex, _output);
	// 線分
	case 2: return SolveLine(_simplex, _output);
	// 三角形
	case 3: return SolveTriangle(_simplex, _output);
	// 四面体
	case 4: return SolveTetrahedron(_simplex, _output);
	// 例外
	default:return false;
	}
}

// 一点の時の計算
bool CollisionSystem::SolvePoint(Simplex& _simplex, Vector3& _output)
{
	// 点が一つなので単純に座標がそのまま方向にする
	_output = -_simplex[0];
	// 当たったかわからないからfalse
	return false;
}

// 二点の時(線分)の計算
bool CollisionSystem::SolveLine(Simplex& _simplex, Vector3& _output)
{
	//        　　　　スタート地点   終点地点
	Vector3 segment{ _simplex[0] - _simplex[1] };
	// スタート地点から原点へのベクトル
	Vector3 startVec{ -_simplex[0] };
	// 三重積を使ってベクトルを求める
	_output = Vector3::Cross(Vector3::Cross(segment, startVec), segment);
	// 当たったかわからないからfalse
	return false;
}

// 三点の時(三角形)の計算
bool CollisionSystem::SolveTriangle(Simplex& _simplex, Vector3& _output)
{
	// 各線分
	Vector3 segments[4]{ 
		 _simplex[0] - _simplex[2],
		 _simplex[1] - _simplex[2],
		 _simplex[0] - _simplex[1],
		 _simplex[2] - _simplex[1]
	};

	// 新たな点と原点ベクトル
	Vector3 toOrigin{ -_simplex[2] };

	// 外積
	Vector3 cross{ Vector3::Cross(segments[0],Vector3::Cross(segments[0],segments[1]))};
	if (Vector3::Dot(toOrigin, cross) > 0)
	{
		_output = cross;

		return false;
	}

	cross = Vector3::Cross(segments[1], Vector3::Cross(segments[1], segments[0]));

	if (Vector3::Dot(toOrigin, cross) > 0)
	{
		_output = cross;

		return false;
	}

	cross = Vector3::Cross(segments[2], Vector3::Cross(segments[2], segments[3]));
	toOrigin = -_simplex[0];

	if (Vector3::Dot(toOrigin, cross) > 0)
	{
		_output = cross;

		return false;
	}


	// 三角形内の領域なので三角形の外積で勝負
	cross = Vector3::Cross(segments[0], segments[1]);

	if (Vector3::Dot(cross, toOrigin) < 0)
	{
		_output = -cross;
	}

	_output = cross;

	return false;
}

// 四点の時(四面体)の計算
bool CollisionSystem::SolveTetrahedron(Simplex& _simplex, Vector3& _output)
{
	// サイズを取得しておく
	int size{ _simplex.GetSize() };
	// ベスト評価
	float bestScore{ FLT_MIN };
	// 各頂点から四面体の四つの面を確認する
	for (int i{ 0 }; i < size; i++)
	{
		// 頂点の検出
		Vector3 a{ _simplex[i] };
		Vector3 b{ _simplex[(i + 1) % size] };
		Vector3 c{ _simplex[(i + 2) % size] };

		// 外積計算から
		Vector3 cross{ Vector3::Cross(b - a,c - a) };
		// 重心
		Vector3 center{ (a + b + c) / 3.0f };

		// 内積使って外側向けてやる
		if (Vector3::Dot(cross, center) < 0)
		{
			cross = -cross;
		}

		// 領域内に原点がいるかチェックしいるなら候補
		if (Vector3::Dot(cross, -center) > 0)
		{
			// 候補なので原点方向に射影してその値で比べる
			float score = Vector3::Dot(cross, -center);

			// 大小判定
			if (score > bestScore)
			{
				bestScore = score;
				_output = cross;
			}
		}
	}

	// ベストスコアが最低値のままなら四面体は原点を内部に含むとして衝突
	if (bestScore == FLT_MIN)
	{
		return true;
	}
	else
	{
		// 最も原点から遠い点を削除する
		bestScore = FLT_MIN;
		int bestIndex{ 0 };
		for (int i{ 0 }; i < size; i++)
		{
			float len{ _simplex[i].LengthSqr() };
			if (len > bestScore)
			{
				bestScore = len;
				bestIndex = i;
			}
		}

		_simplex.Erase(bestIndex);
		return false;
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

	projectionStorage.Get(_projectionValues[_a].entity)->endpointIndex[_projectionValues[_a].axisType] = _a;
	projectionStorage.Get(_projectionValues[_b].entity)->endpointIndex[_projectionValues[_b].axisType] = _b;
}
