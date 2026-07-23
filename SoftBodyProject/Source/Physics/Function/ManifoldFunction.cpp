#include <algorithm>

#include "ManifoldFunction.h"

void ManifoldFunction::AddUniquePoint(Manifold& _manifold, const ContactPoint& _point)
{
    // 近すぎる点は追加しない
    for (int i = 0; i < _manifold.pointCount; i++)
    {
        if (Vector3::DistanceSqr(_manifold.points[i].positionLocalA, _point.positionLocalA) < MathConstants::EPSILON * MathConstants::EPSILON)
        {
            return;
        }
    }

    _manifold.AddPoints(_point);
}

void ManifoldFunction::BoxBox(
    const Vector3& _positionA, const Quaternion& _rotationA, const Vector3* _candidateAxisA, const float* _halfsA,
    const Vector3& _positionB, const Quaternion& _rotationB, const Vector3* _candidateAxisB, const float* _halfsB,
    const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
{
    switch (_info.type)
    {
    case BoxBoxContactInfo::FaceA:
        AddFaceAManifold(
            _positionA, _rotationA, _candidateAxisA, _halfsA,
            _positionB, _rotationB, _candidateAxisB, _halfsB,
            _info, _manifoldBuffer);
        break;
    case BoxBoxContactInfo::FaceB:
        AddFaceBManifold(
            _positionA, _rotationA, _candidateAxisA, _halfsA,
            _positionB, _rotationB, _candidateAxisB, _halfsB,
            _info, _manifoldBuffer);
        break;
    case BoxBoxContactInfo::EdgeEdge:
        AddEdgeManifold(
            _positionA, _rotationA, _candidateAxisA, _halfsA,
            _positionB, _rotationB, _candidateAxisB, _halfsB,
            _info, _manifoldBuffer);
        break;
    }
}

void ManifoldFunction::AddFaceAManifold(
    const Vector3& _positionA, const Quaternion& _rotationA, const Vector3* _candidateAxisA, const float* _halfsA,
    const Vector3& _positionB, const Quaternion& _rotationB, const Vector3* _candidateAxisB, const float* _halfsB,
    const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
{
    Manifold manifold;
    manifold.colliderA = _info.colliderA;
    manifold.colliderB = _info.colliderB;
    manifold.normal = _info.normal;

    // 基準の面の法線
    Vector3 faceNormal{ _candidateAxisA[_info.axisA] };

    // 法線をAからB向きにそろえる
    if (Vector3::Dot(faceNormal, _info.normal) < 0.0f)
    {
        faceNormal = -faceNormal;
    }

    // 基準面の中心
    Vector3 faceCenter{ _positionA + faceNormal * _halfsA[_info.axisA] };

    std::vector<Vector3> poly{ GenerateFaceContact(
        _positionA, _candidateAxisA, _halfsA,
        _positionB, _candidateAxisB, _halfsB,
        faceNormal, faceCenter, _info.axisA
    ) };

    // 得られたBの位置からAの位置も作る
    for (const Vector3& positionB : poly)
    {
        float penetration{ Vector3::Dot(positionB - faceCenter, faceNormal) };

        if (penetration <= 0.0f)
        {
            ContactPoint contactPoint;

            contactPoint.positionLocalA = _rotationA.Conjugate().Rotate((positionB - faceNormal * penetration) - _positionA);
            contactPoint.positionLocalB = _rotationB.Conjugate().Rotate(positionB - _positionB);
            contactPoint.penetration = -penetration;

            AddUniquePoint(manifold, contactPoint);
        }
    }

    if (manifold.pointCount <= 0)
    {
        return;
    }

    _manifoldBuffer->manifolds.push_back(manifold);
}
void ManifoldFunction::AddFaceBManifold(
    const Vector3& _positionA, const Quaternion& _rotationA, const Vector3* _candidateAxisA, const float* _halfsA,
    const Vector3& _positionB, const Quaternion& _rotationB, const Vector3* _candidateAxisB, const float* _halfsB,
    const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
{
    Manifold manifold;
    manifold.colliderA = _info.colliderA;
    manifold.colliderB = _info.colliderB;
    manifold.normal = _info.normal;

    // 基準の面の法線
    Vector3 faceNormal{ _candidateAxisB[_info.axisB] };

    // 法線をBからA向きにそろえる
    if (Vector3::Dot(faceNormal, _info.normal) > 0.0f)
    {
        faceNormal = -faceNormal;
    }

    // 基準面の中心
    Vector3 faceCenter{ _positionB + faceNormal * _halfsB[_info.axisB] };


    std::vector<Vector3> poly{ GenerateFaceContact(
        _positionB, _candidateAxisB, _halfsB,
        _positionA, _candidateAxisA, _halfsA,
        faceNormal, faceCenter, _info.axisB
    ) };

    // 得られたBの位置からAの位置も作る
    for (const Vector3& positionA : poly)
    {
        float penetration{ Vector3::Dot(positionA - faceCenter, faceNormal) };

        if (penetration <= 0.0f)
        {
            ContactPoint cp;

            cp.positionLocalA = _rotationA.Conjugate().Rotate(positionA - _positionA);
            cp.positionLocalB = _rotationB.Conjugate().Rotate((positionA - faceNormal * penetration) - _positionB);
            cp.penetration = -penetration;

            AddUniquePoint(manifold, cp);
        }
    }

    if (manifold.pointCount <= 0)
    {
        return;
    }

    _manifoldBuffer->manifolds.push_back(manifold);
}

void ManifoldFunction::AddEdgeManifold(
    const Vector3& _positionA, const Quaternion& _rotationA, const Vector3* _candidateAxisA, const float* _halfsA,
    const Vector3& _positionB, const Quaternion& _rotationB, const Vector3* _candidateAxisB, const float* _halfsB,
    const BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
{
    Manifold manifold;
    manifold.colliderA = _info.colliderA;
    manifold.colliderB = _info.colliderB;
    manifold.normal = _info.normal;

    Vector3 edgeCenterA{ _positionA };
    Vector3  edgeCenterB{ _positionB };

    for (int i = 0; i < 3; ++i)
    {
        if (i != _info.axisA)
        {
            float sign = (Vector3::Dot(_candidateAxisA[i], _info.normal) >= 0.0f) ? 1.0f : -1.0f;
            edgeCenterA += _candidateAxisA[i] * (_halfsA[i] * sign);
        }

        if (i != _info.axisB)
        {
            float sign = (Vector3::Dot(_candidateAxisB[i], _info.normal) >= 0.0f) ? -1.0f : 1.0f;
            edgeCenterB += _candidateAxisB[i] * (_halfsB[i] * sign);
        }
    }

    // 各辺の端を計算
    Vector3 startA = edgeCenterA - _candidateAxisA[_info.axisA] * _halfsA[_info.axisA];
    Vector3 endA = edgeCenterA + _candidateAxisA[_info.axisA] * _halfsA[_info.axisA];
    Vector3 startB = edgeCenterB - _candidateAxisB[_info.axisB] * _halfsB[_info.axisB];
    Vector3 endB = edgeCenterB + _candidateAxisB[_info.axisB] * _halfsB[_info.axisB];

    // 辺ベクトル
    Vector3 segmentA = endA - startA;
    Vector3 segmentB = endB - startB;
    // AとBのスタート点のベクトル
    Vector3 r = startA - startB;

    // 長さ
    float lengthA = Vector3::Dot(segmentA, segmentA);
    float lengthB = Vector3::Dot(segmentB, segmentB);
    // スタート点をつなぐベクトルを射影
    float dotB = Vector3::Dot(segmentB, r);
    float dotA = Vector3::Dot(segmentA, r);

    // 最近点用の係数
    float s = 0.0f;
    float t = 0.0f;

    // 両方の長さが0に近いなら、スタート点を使う
    if (lengthA <= MathConstants::EPSILON && lengthB <= MathConstants::EPSILON)
    {
        s = 0.0f;
        t = 0.0f;
    }
    // Aの長さが0に近いなら、Aはスタート点Bは最近点
    else if (lengthA <= MathConstants::EPSILON)
    {
        s = 0.0f;
        t = std::clamp((dotB / lengthB), 0.0f, 1.0f);
    }
    // Bの長さが0に近いなら、Bはスタート点Aは最近点
    else if (lengthB <= MathConstants::EPSILON)
    {
        t = 0.0f;
        s = std::clamp((-dotA / lengthA), 0.0f, 1.0f);
    }
    // 両者長さが十分なら普通に最近点を求める
    else
    {
        /*
               最近点ということは、最近点同士を結んだベクトルとAとBの辺の内積は0になるはず
                 dot(dotA A(s) - B(t)) = 0
                 dot(dotB, A(s) - B(t)) = 0
                 になるsとtを求める
                 連立方程式として解いたら下のような式になる
        */
        float dotAB = Vector3::Dot(segmentA, segmentB);
        float denom = lengthA * lengthB - dotAB * dotAB;

        if (std::abs(denom) > MathConstants::EPSILON)
        {
            s = std::clamp(((dotAB * dotB - dotA * lengthB) / denom), 0.0f, 1.0f);
        }
        else
        {
            s = 0.0f;
        }

        t = (dotAB * s + dotB) / lengthB;

        if (t < 0.0f)
        {
            t = 0.0f;
            s = std::clamp((-dotA / lengthA), 0.0f, 1.0f);
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
            s = std::clamp(((dotAB - dotA) / lengthA), 0.0f, 1.0f);
        }
    }

    // 求まったs.tから最近点を計算
    Vector3 closestA = startA + segmentA * s;
    Vector3 closestB = startB + segmentB * t;

    ContactPoint cp;
    cp.positionLocalA = _rotationA.Conjugate().Rotate(closestA - _positionA);
    cp.positionLocalB = _rotationB.Conjugate().Rotate(closestB - _positionB);
    cp.penetration = _info.depth;

    // 追加
    AddUniquePoint(manifold, cp);

    if (manifold.pointCount <= 0)
    {
        return;
    }

    // バッファに追加
    _manifoldBuffer->manifolds.push_back(manifold);
}


std::vector<Vector3> ManifoldFunction::GenerateFaceContact(
    const Vector3& _refarencePos, const Vector3* const _refarenceAxis, const  float* _refarenceHalfs,
    const Vector3& _incidentPos, const Vector3* const _incidentAxis, const float* _incidentHalfs,
    const Vector3& _faceNormal, const Vector3& _faceCenter, int _refarenceIndex)
{
    // 基準面の横幅と縦幅の方向と大きさを用意
    int vIndex{ (_refarenceIndex + 1) % 3 };
    int hIndex{ (_refarenceIndex + 2) % 3 };

    Vector3 vNormal{ _refarenceAxis[vIndex] };
    Vector3 hNormal{ _refarenceAxis[hIndex] };

    float vHalf{ _refarenceHalfs[vIndex] };
    float hHalf{ _refarenceHalfs[hIndex] };

    // normalと一番逆向きの入射面の法線を探す
    int bIndex{ 0 };
    float bestDot{ std::abs(Vector3::Dot(_incidentAxis[0], _faceNormal)) };

    for (int i{ 1 }; i < 3; ++i)
    {
        float d{ std::abs(Vector3::Dot(_incidentAxis[i], _faceNormal)) };
        if (d > bestDot)
        {
            bestDot = d;
            bIndex = i;
        }
    }

    Vector3 bNormal{ _incidentAxis[bIndex] };

    if (Vector3::Dot(bNormal, _faceNormal) > 0.0f)
    {
        bNormal = -bNormal;
    }

    // 入射面の中心
    Vector3 bCenter{ _incidentPos + bNormal * _incidentHalfs[bIndex] };

    // 選ばれた法線の横と縦に該当するIndex
    int bVIndex{ (bIndex + 1) % 3 };
    int bHIndex{ (bIndex + 2) % 3 };


    // 入射側の選ばれた面の4頂点
    std::vector<Vector3> poly;
    poly.reserve(4);

    poly.push_back(bCenter + _incidentAxis[bVIndex] * _incidentHalfs[bVIndex] + _incidentAxis[bHIndex] * _incidentHalfs[bHIndex]);
    poly.push_back(bCenter - _incidentAxis[bVIndex] * _incidentHalfs[bVIndex] + _incidentAxis[bHIndex] * _incidentHalfs[bHIndex]);
    poly.push_back(bCenter - _incidentAxis[bVIndex] * _incidentHalfs[bVIndex] - _incidentAxis[bHIndex] * _incidentHalfs[bHIndex]);
    poly.push_back(bCenter + _incidentAxis[bVIndex] * _incidentHalfs[bVIndex] - _incidentAxis[bHIndex] * _incidentHalfs[bHIndex]);

    // 入射面を基準面内に収める
    poly = ClipFace(poly, vNormal, Vector3::Dot(vNormal, _faceCenter) + vHalf);
    poly = ClipFace(poly, -vNormal, Vector3::Dot(-vNormal, _faceCenter) + vHalf);
    poly = ClipFace(poly, hNormal, Vector3::Dot(hNormal, _faceCenter) + hHalf);
    poly = ClipFace(poly, -hNormal, Vector3::Dot(-hNormal, _faceCenter) + hHalf);

    return poly;
}

std::vector<Vector3> ManifoldFunction::ClipFace(const std::vector<Vector3>& _input, const Vector3& _faceNormal, float _planeOffset)
{
    std::vector<Vector3> output;

    for (int i = 0; i < _input.size(); ++i)
    {
        // 辺を作る
        Vector3 start = _input[i];
        Vector3 end = _input[(i + 1) % _input.size()];

        float dStart = Vector3::Dot(_faceNormal, start) - _planeOffset;
        float dEnd = Vector3::Dot(_faceNormal, end) - _planeOffset;

        // 面の中にあるか判定
        bool insideStart = dStart <= 0.0f;
        bool insideEnd = dEnd <= 0.0f;

        // 面の中にあった点の対応によって、追加する点を変える
        if (insideStart && insideEnd)
        {
            output.push_back(end);
        }
        else if (insideStart && !insideEnd)
        {
            float t = dStart / (dStart - dEnd);
            output.push_back(start + (end - start) * t);
        }
        else if (!insideStart && insideEnd)
        {
            float t = dStart / (dStart - dEnd);
            output.push_back(start + (end - start) * t);
            output.push_back(end);
        }
    }

    return output;
}
