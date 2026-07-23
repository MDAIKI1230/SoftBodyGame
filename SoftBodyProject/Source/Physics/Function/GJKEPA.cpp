#include "MinkowskiVertex.h"
#include "Simplex.h"
#include "CollisionPair.h"

#include "GJKEPA.h"

template<class A, class B>
bool GJKEPA::GJK(
	ColliderID _colliderA, ColliderID _colliderB,
	ColliderStorage* _colliderStorage,
	PhysicsTransformStorage* _transformStorage,
	CollisionManifoldBuffer* _manifoldBuffer)
{
	Simplex simplex{};
	Vector3 dir{ Vector3::UP };

	while (true)
	{
		// ミンコフスキー差の支点計算
		MinkowskiVertex vertex;
		vertex.supportA = A::Support(_colliderStorage, _colliderA, _transformStorage, dir);
		vertex.supportB = B::Support(_colliderStorage, _colliderB, _transformStorage, -dir);
		vertex.minkowski = { vertex.supportA - vertex.supportB };

		// 支点をSimplexに追加
		simplex.Add(vertex);

		// 内積から当たる可能性があるのか見てみる
		if (Vector3::Dot(vertex.minkowski, dir) < 0)
		{
			return false;
		}

		if (SimplexSolve(simplex, dir))
		{
			EPA<A, B>(_colliderA, _colliderB, _transformStorage, _colliderStorage, _manifoldBuffer, simplex);
			return true;
		}
	}

	return false;
}

bool GJKEPA::SimplexSolve(Simplex& _simplex, Vector3& _output)
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
bool GJKEPA::SolvePoint(Simplex& _simplex, Vector3& _output)
{
	// 点が一つなので単純に座標がそのまま方向にする
	_output = -_simplex[0].minkowski;
	// 当たったかわからないからfalse
	return false;
}

// 二点の時(線分)の計算
bool GJKEPA::SolveLine(Simplex& _simplex, Vector3& _output)
{
	//        　　　　スタート地点   終点地点
	Vector3 segment{ _simplex[0].minkowski - _simplex[1].minkowski };
	// スタート地点から原点へのベクトル
	Vector3 startVec{ -_simplex[0].minkowski };
	// 三重積を使ってベクトルを求める
	_output = Vector3::Cross(Vector3::Cross(segment, startVec), segment);
	// 当たったかわからないからfalse
	return false;
}

// 三点の時(三角形)の計算
bool GJKEPA::SolveTriangle(Simplex& _simplex, Vector3& _output)
{
	// 各線分
	Vector3 segments[4]{
		 _simplex[0].minkowski - _simplex[2].minkowski,
		 _simplex[1].minkowski - _simplex[2].minkowski,
		 _simplex[0].minkowski - _simplex[1].minkowski,
		 _simplex[2].minkowski - _simplex[1].minkowski
	};

	// 新たな点と原点ベクトル
	Vector3 toOrigin{ -_simplex[2].minkowski };

	// 外積
	Vector3 cross{ Vector3::Cross(segments[0],Vector3::Cross(segments[0],segments[1])) };
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
	toOrigin = -_simplex[0].minkowski;

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

		return false;
	}

	_output = cross;

	return false;
}

// 四点の時(四面体)の計算
bool GJKEPA::SolveTetrahedron(Simplex& _simplex, Vector3& _output)
{
	// サイズを取得しておく
	int size{ _simplex.GetSize() };
	// ベスト評価
	float bestScore{ FLT_MIN };
	int index{ 0 };
	// 各頂点から四面体の四つの面を確認する
	for (int i{ 0 }; i < size; i++)
	{
		// 頂点の検出
		Vector3 a{ _simplex[i].minkowski };
		Vector3 b{ _simplex[(i + 1) % size].minkowski };
		Vector3 c{ _simplex[(i + 2) % size].minkowski };

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

				index = i;
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
		_simplex.Erase((index + 3) % size);
		return false;
	}
}


template<class A, class B>
void GJKEPA::EPA(
	ColliderID _colliderA, ColliderID _colliderB,
	PhysicsTransformStorage* _transformStorage,
	ColliderStorage* _colliderStorage,
	CollisionManifoldBuffer* _manifoldBuffer, const Simplex& _simplex)
{
	std::vector<MinkowskiVertex> vertices{ _simplex[0],_simplex[1], _simplex[2], _simplex[3] };
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
		// ミンコフスキー差の支点計算
		MinkowskiVertex vertex;
		vertex.supportA = A::Support(_colliderStorage, _colliderA, _transformStorage, faces[minIndex].normal);
		vertex.supportB = B::Support(_colliderStorage, _colliderB, _transformStorage, -faces[minIndex].normal);
		vertex.minkowski = { vertex.supportA - vertex.supportB };

		// 収束判定
		if (std::abs(Vector3::Dot(faces[minIndex].normal, vertex.minkowski) - faces[minIndex].distance) < MathConstants::EPSILON)
		{
			CollisionPair::Pair pair{ _colliderA, _colliderB };
			Manifold manifold;
			manifold.colliderA = _colliderA;
			manifold.colliderB = _colliderB;
			manifold.normal = faces[minIndex].normal.Normalize();

			ContactPoint contactPoint{ CalcContactPosition(faces[minIndex], vertices) };
			contactPoint.penetration = faces[minIndex].distance;
			manifold.AddPoints(contactPoint);

			_manifoldBuffer->manifolds.push_back(manifold);

			return;
		}

		vertices.push_back(vertex);

		// エッジを追加
		for (int i{ (int)faces.size() - 1 }; i >= 0; i--)
		{
			// 内積で、新たな点を向いてる面を出す。
			if (Vector3::Dot(faces[i].normal, vertex.minkowski - vertices[faces[i].pointIndex[0]].minkowski) > 0)
			{
				// 新しいエッジ追加
				for (int j{ 0 }; j < 3; j++)
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

		if (count > 10)
		{
			CollisionPair::Pair pair{ _colliderA, _colliderB };
			// 探してあったら情報を修正・追加無かったら新たに作る
			Manifold manifold;
			manifold.colliderA = _colliderA;
			manifold.colliderB = _colliderB;
			manifold.normal = faces[minIndex].normal.Normalize();

			ContactPoint contactPoint{ CalcContactPosition(faces[minIndex], vertices) };
			contactPoint.penetration = faces[minIndex].distance;
			manifold.AddPoints(contactPoint);

			_manifoldBuffer->manifolds.push_back(manifold);

			return;
		}
	}
}

void GJKEPA::ComputeFace(Face& _face, const std::vector<MinkowskiVertex>& _vertices)
{
	// 頂点の検出
	Vector3 a{ _vertices[_face.pointIndex[0]].minkowski };
	Vector3 b{ _vertices[_face.pointIndex[1]].minkowski };
	Vector3 c{ _vertices[_face.pointIndex[2]].minkowski };

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

void GJKEPA::AddEdge(Edge _edge, std::vector<Edge>& _edges)
{
	for (int i{ 0 }; i < _edges.size(); i++)
	{
		if (_edges[i].a == _edge.b &&
			_edges[i].b == _edge.a)
		{
			_edges[i] = _edges.back();
			_edges.pop_back();

			return;
		}
	}

	_edges.push_back(_edge);
}

ContactPoint GJKEPA::CalcContactPosition(const Face& _face, const std::vector<MinkowskiVertex>& _vertices)
{
	Vector3 closestPoint{ _face.normal * _face.distance };
	Vector3 v0{ _vertices[_face.pointIndex[1]].minkowski - _vertices[_face.pointIndex[0]].minkowski };
	Vector3	v1{ _vertices[_face.pointIndex[2]].minkowski - _vertices[_face.pointIndex[0]].minkowski };
	Vector3	v2{ closestPoint - _vertices[_face.pointIndex[0]].minkowski };

	float d00{ Vector3::Dot(v0, v0) };
	float d01{ Vector3::Dot(v0, v1) };
	float d11{ Vector3::Dot(v1, v1) };
	float d20{ Vector3::Dot(v2, v0) };
	float d21{ Vector3::Dot(v2, v1) };

	float denom{ d00 * d11 - d01 * d01 };

	float w1{ (d11 * d20 - d01 * d21) / denom };
	float w2{ (d00 * d21 - d01 * d20) / denom };
	float w0{ 1 - w1 - w2 };

	Vector3 pointA{ _vertices[_face.pointIndex[0]].supportA * w0 + _vertices[_face.pointIndex[1]].supportA * w1 + _vertices[_face.pointIndex[2]].supportA * w2 };
	Vector3 pointB{ _vertices[_face.pointIndex[0]].supportB * w0 + _vertices[_face.pointIndex[1]].supportB * w1 + _vertices[_face.pointIndex[2]].supportB * w2 };


	return ContactPoint{ pointA , pointB };
}
