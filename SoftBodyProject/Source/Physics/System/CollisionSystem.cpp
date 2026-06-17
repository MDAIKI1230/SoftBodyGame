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
	// 参照用
	TransformComponent transA{}, transB{};

	// 
	for (auto& pair : narrowPairs)
	{
		// Transformがあるかチェックないなら飛ばす
		if (!transformStorage->TryGet(pair.a, transA))
		{
			continue;
		}
		if (!transformStorage->TryGet(pair.b, transB))
		{
			continue;
		}

		// 各コライダー取得
		ColliderComponent* collA{ colliderStorage->Get(pair.a) };
		ColliderComponent* collB{ colliderStorage->Get(pair.a) };

		if (GJK(*collA, transA, *collB, transB))
		{

		}
	}
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

	Simplex simplex{};
	Vector3 dir{ Vector3::UP };

	while (true)
	{
		// ミンコフスキー差の支点計算
		Vector3 vec{ collider01.Support(dir) - collider02.Support(-dir) + (transform02.GetPosition() - transform01.GetPosition()) };

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

	//// 上方向の支点を最初の支点とする
	//Vector3 dir{ Vector3::UP };
	//// ミンコフスキー差の支点(上方向)
	//Vector3 fulcrums[3]
	//{
	//	// 初期の点以外はゼロで初期化
	//	collider01.Support(dir) - collider02.Support(-dir) + (transform02.GetPosition() - transform01.GetPosition())
	//	,Vector3::ZERO,Vector3::ZERO
	//};

	//// どのインデックスに点を入れるのか(今、0,1には初期点とその点から生成した方向とその時の支点が入っているので最後の2の要素がスタート)
	//char index{ 2 };
	//// 新しく出した支点とその時の方向の内積が0以下なら原点にこれ以上向かう事が出来ないとして終了
	//while (dot <= 0.0f)
	//{
	//	// 点の生成
	//	// 既存の2つの点から線分を生成(インデックスは決めるインデックスの次とその次)
	//	
	//	// この新しい方向から新たな点を計算
	//	fulcrums[index] = collider01.Support(dir) - collider02.Support(-dir) + (transform02.GetPosition() - transform01.GetPosition());

	//	// 4面体内に原点が含まれているか判定

	//}

	return false;
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
		return false;
	}
}