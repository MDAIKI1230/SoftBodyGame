#include "ManifoldFunction.h"

void ManifoldFunction::AddUniquePoint(Manifold& _manifold, const ContactPoint& _point)
{
    // 近すぎる点は追加しない
    for (int i = 0; i < _manifold.pointCount; i++)
    {
        if (Vector3::DistanceSqr(_manifold.points[i].positionA, _point.positionA) < MathConstants::EPSILON * MathConstants::EPSILON)
        {
            return;
        }
    }

    _manifold.AddPoints(_point);
}

void ManifoldFunction::BoxBox(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
{
    switch (_info.type)
    {
    case BoxBoxContactInfo::FaceA:
        AddFaceAManifold(_positionA, _positionB, _candidateAxisA, _candidateAxisB, _halfsA, _halfsB, _info, _manifoldBuffer);
        break;
    case BoxBoxContactInfo::FaceB:
        AddFaceBManifold(_positionA, _positionB, _candidateAxisA, _candidateAxisB, _halfsA, _halfsB, _info, _manifoldBuffer);
        break;
    case BoxBoxContactInfo::EdgeEdge:

        break;
    }
}

void ManifoldFunction::AddFaceAManifold(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
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
            ContactPoint cp;

            cp.positionB = positionB;
            cp.positionA = positionB - faceNormal * penetration;
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
void ManifoldFunction::AddFaceBManifold(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
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

            cp.positionB = positionA - faceNormal * penetration;
            cp.positionA = positionA;
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
void ManifoldFunction::AddEdgeManifold(Vector3& _positionA, Vector3& _positionB, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB, BoxBoxContactInfo& _info, CollisionManifoldBuffer* _manifoldBuffer)
{

}


std::vector<Vector3> ManifoldFunction::GenerateFaceContact(
    Vector3& _refarencePos, Vector3* _refarenceAxis, float* _refarenceHalfs,
    Vector3& _incidentPos, Vector3* _incidentAxis, float* _incidentHalfs,
    Vector3& _faceNormal, Vector3& _faceCenter, int _refarenceIndex)
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
