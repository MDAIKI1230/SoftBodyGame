#include <algorithm>

#include "ColliderTags.h"
#include "PhysicsEvent.h"

#include "CollisionSystem.h"

void CollisionSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, EventManager* _eventManager, CollisionManifoldBuffer* _manifoldBuffer)
{
	// --- 衝突処理 --- 
	BroadPhase(_transformStorage, _colliderStorage);
	Dispatch(_colliderStorage);
	NarrowPhase(_transformStorage, _colliderStorage, _eventManager, _manifoldBuffer);

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

		data.min = _colliderStorage->aabbStorage->aabb[i].min + _transformStorage->position[transformIndex];
		data.max = _colliderStorage->aabbStorage->aabb[i].max + _transformStorage->position[transformIndex];

		// コライダーハンドルのindex
		size_t colliderIndex{ aabbStorage->aabb[i].colliderID.index };

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

void CollisionSystem::NarrowPhase(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, EventManager* _eventManager, CollisionManifoldBuffer* _manifoldBuffer)
{
	// 球VS球
	Solve<ColliderTag::SphereTag, ColliderTag::SphereTag, SphereColliderStorage, SphereColliderStorage, CollisionPair::SphereSpherePair>
		(
			_colliderStorage->sphereStorage.get(), _colliderStorage->sphereStorage.get(),
			narrowPhasePairBuilder.sphereSpherePair,
			_transformStorage,
			_colliderStorage,
			_eventManager,
			_manifoldBuffer
		);

	// 箱VS球
	Solve<ColliderTag::BoxTag, ColliderTag::SphereTag, BoxColliderStorage, SphereColliderStorage, CollisionPair::BoxSpherePair>
		(
			_colliderStorage->boxStorage.get(), _colliderStorage->sphereStorage.get(),
			narrowPhasePairBuilder.boxSpherePair,
			_transformStorage,
			_colliderStorage,
			_eventManager,
			_manifoldBuffer
		);

	// 球VS箱
	Solve<ColliderTag::SphereTag, ColliderTag::BoxTag, SphereColliderStorage, BoxColliderStorage, CollisionPair::SphereBoxPair>
		(
			_colliderStorage->sphereStorage.get(), _colliderStorage->boxStorage.get(),
			narrowPhasePairBuilder.sphereBoxPair,
			_transformStorage,
			_colliderStorage,
			_eventManager,
			_manifoldBuffer
		);

	// 箱VS箱
	Solve<ColliderTag::BoxTag, ColliderTag::BoxTag, BoxColliderStorage, BoxColliderStorage, CollisionPair::BoxBoxPair>
		(
			_colliderStorage->boxStorage.get(), _colliderStorage->boxStorage.get(),
			narrowPhasePairBuilder.boxBoxPair,
			_transformStorage,
			_colliderStorage,
			_eventManager,
			_manifoldBuffer
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
				if (_projectionAxisValues[i].colliderID.index > active)
				{
					// 軸で交差しているので交差カウント増加
					crossCountMap[CollisionPair::BroadPhasePair(_projectionAxisValues[i].colliderID, _projectionAxisValues[active].colliderID)] += 1;
				}
				else
				{
					// 軸で交差しているので交差カウント増加
					crossCountMap[CollisionPair::BroadPhasePair(_projectionAxisValues[active].colliderID, _projectionAxisValues[i].colliderID)] += 1;
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
			uint32_t minAxisIndex{ projectionDatas[_projectionAxisValues[i].colliderID.index].endpointIndex[minAxis] };

			// 移動した奴のIndexの更新
			_projectionAxisValues[actives.back()].activeIndex = _projectionAxisValues[minAxisIndex].activeIndex;

			actives[_projectionAxisValues[minAxisIndex].activeIndex] = actives.back();
			actives.pop_back();
		}
	}
}

template<class A, class B, class AS, class BS, class PairList>
void CollisionSystem::Solve(
	AS* _strageA, BS* _strageB,
	const std::vector<PairList>& pairList,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	EventManager* _eventManager,
	CollisionManifoldBuffer* _manifoldBuffer)
{
	// ナローフェーズに行けたペアの衝突判定をしていく
	for (auto& pair : pairList)
	{
		if (GJK<A, B>(pair.a, pair.b, _colliderStorage, _transformStorage, _manifoldBuffer))
		{
			RegisterEvent(pair.a, pair.b, _colliderStorage, _eventManager);
		}
	}
}

template<class A, class B>
bool CollisionSystem::GJK(
	ColliderID _handleA, ColliderID _handleB,
	ColliderStorage* _colliderStorage,
	PhysicsTransformStorage* _transformStorage,
	CollisionManifoldBuffer* _manifoldBuffer)
{
	Simplex simplex{};
	Vector3 dir{ Vector3::UP };

	while (true)
	{
		// ミンコフスキー差の支点計算
		Vector3 vec{ A::Support(_colliderStorage,_handleA, _transformStorage,dir) - B::Support(_colliderStorage,_handleB,_transformStorage ,-dir) };

		// 支点をSimplexに追加
		simplex.Add(vec);

		// 内積から当たる可能性があるのか見てみる
		if (Vector3::Dot(vec, dir) < 0)
		{
			return false;
		}

		if(SimplexSolve(simplex,dir))
		{
			EPA<A, B>(_handleA, _handleB, _transformStorage, _colliderStorage, _manifoldBuffer, simplex);
			return true;
		}
	}

	return false;
}

void CollisionSystem::RegisterEvent(
	ColliderID _a,
	ColliderID _b,
	ColliderStorage* _colliderStorage,
	EventManager* _eventManager)
{
	// 今回のペア追加
	currentFramePair.insert(CollisionPair::BroadPhasePair{ _a, _b });

	// 当たっているのでとりあえずよべる
	_eventManager->Push<OnCollisionEvent>({ _colliderStorage->GetOwnerEntity(_a),_colliderStorage->GetOwnerEntity(_b) });

	// 前回のフレーム当たってなくて今回当たってるため衝突開始のイベントを呼ぶ
	if (!prevFramePair.contains(CollisionPair::BroadPhasePair{ _a, _b }))
	{
		_eventManager->Push<OnCollisionEnterEvent>({ _colliderStorage->GetOwnerEntity(_a),_colliderStorage->GetOwnerEntity(_b) });
	}
}

void CollisionSystem::RegisterExitEvent(ColliderStorage* _colliderStorage, EventManager* _eventManager)
{
	std::unordered_set<CollisionPair::BroadPhasePair> erasePair;

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

template<class A, class B>
void CollisionSystem::EPA(
	ColliderID _handleA, ColliderID _handleB,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer, Simplex& _simplex)
{
	std::vector<Vector3> vertices{ _simplex[0],_simplex[1], _simplex[2], _simplex[3] };
	std::vector<Face> faces;
	std::vector<Edge> edges;

	// --- 今の各面のSimplexのFace計算 ---

	// Indexを埋めてFaceを作る
	for (int i{ 0 }; i < _simplex.GetSize(); i++)
	{
		unsigned char aIndex{ (unsigned char)i };
		unsigned char bIndex{ (unsigned char)((i + 1) % _simplex.GetSize()) };
		unsigned char cIndex{ (unsigned char)((i + 2) % _simplex.GetSize()) };

		faces.push_back(Face({ aIndex, bIndex, cIndex }));
	}

	// 最短探索用
	int minIndex{ 0 };
	float minDist{ FLT_MAX };

	// 法線、距離計算かつ最短を探す
	for (int i{ 0 }; i < _simplex.GetSize(); i++)
	{
		ComputeFace(faces[i], vertices);

		if (minDist > faces[i].distance)
		{
			minDist = faces[i].distance;
			minIndex = i;
		}
	}

	int count{ 0 };
	// EPAのループ
	while (true)
	{
		edges.clear();

		// サポート関数を使って新たな点を計算
		Vector3 support{ A::Support(_colliderStorage,_handleA, _transformStorage,faces[minIndex].normal) - B::Support(_colliderStorage,_handleB,_transformStorage ,-faces[minIndex].normal) };
		
		// 収束判定
		if (std::abs(Vector3::Dot(faces[minIndex].normal, support) - faces[minIndex].distance) < MathConstants::EPSILON)
		{
			Manifold manifold;
			manifold.colliderA = _handleA;
			manifold.colliderB = _handleB;
			manifold.normal = faces[minIndex].normal;
			manifold.points[0].penetration = faces[minIndex].distance;
			manifold.pointCount = 1;

			_manifoldBuffer->manifolds.push_back(manifold);

			return;
		}

		vertices.push_back(support);

		// エッジを追加
		for (int i{ (int)faces.size() - 1 }; i >= 0; i--)
		{
			// 内積で、新たな点を向いてる面を出す。
			if (Vector3::Dot(faces[i].normal, support - vertices[faces[i].pointIndex[0]]) > 0)
			{
				// 新しいエッジ追加
				for (int j{ 0 }; j < 3;j++)
				{
					Edge edge{ (unsigned char)faces[i].pointIndex[j], (unsigned char)faces[i].pointIndex[(j + 1) % 3] };
					// エッジ追加
					AddEdge(edge, edges);
				}

				// 面の削除
				faces.erase(faces.begin() + i);
			}
		}
	
		// エッジから新たな面生成
		for (int j{ 0 }; j < edges.size(); j++)
		{
			faces.push_back(Face({ (unsigned char)(vertices.size() - 1), edges[j].a, edges[j].b }));
			ComputeFace(faces.back(), vertices);
		}

		// 最小判定
		minDist = FLT_MAX;
		minIndex = 0;
		for (int i{ 0 }; i < faces.size(); i++)
		{
			if (minDist > faces[i].distance)
			{
				minDist = faces[i].distance;
				minIndex = i;
			}
		}

		count++;

		if (count > 20)
		{
			Manifold manifold;
			manifold.colliderA = _handleA;
			manifold.colliderB = _handleB;
			manifold.normal = faces[minIndex].normal;
			manifold.points[0].penetration = faces[minIndex].distance;
			manifold.pointCount = 1;

			_manifoldBuffer->manifolds.push_back(manifold);

			return;
		}
	}
}

void CollisionSystem::ComputeFace(Face& _face, std::vector<Vector3>& _vertices)
{
	// 頂点の検出
	Vector3 a{ _vertices[_face.pointIndex[0]] };
	Vector3 b{ _vertices[_face.pointIndex[1]] };
	Vector3 c{ _vertices[_face.pointIndex[2]] };

	// 外積計算から
	Vector3 cross{ Vector3::Cross(b - a, c - a) };
	_face.normal = Vector3::Normalize(cross);

	// 外側向ける
	if (Vector3::Dot(_face.normal, a) < 0.0f)
	{
		std::swap(_face.pointIndex[1], _face.pointIndex[2]);

		_face.normal = -_face.normal;
	}

	// 距離計算
	_face.distance = Vector3::Dot(_face.normal, a);
}

void CollisionSystem::AddEdge(Edge& _edge, std::vector<Edge>& _edges)
{
	for (int i{ 0 }; i < _edges.size(); i++)
	{
		if (_edges[i].a == _edge.b &&
			_edges[i].b == _edge.a)
		{
			_edges[i] = _edges.back();
			_edges.pop_back();

			break;
		}
		else if (_edges[i].a == _edge.a &&
			_edges[i].b == _edge.b)
		{
			_edges[i] = _edges.back();
			_edges.pop_back();

			break;
		}
	}

	_edges.push_back(_edge);
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

	projectionDatas[_projectionValues[_a].colliderID.index].endpointIndex[_projectionValues[_a].axisType] = _a;
	projectionDatas[_projectionValues[_b].colliderID.index].endpointIndex[_projectionValues[_b].axisType] = _b;
}
