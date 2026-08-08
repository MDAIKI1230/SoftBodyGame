#include <algorithm>
#include <array>
#include <cmath>

#include "MDMath.h"

// 線分上の点の最近点
Vector3 MDMath::ClosestPointOnSegment(const Vector3& _point, const Vector3& _start, const Vector3& _end)
{
	Vector3 segment = _end - _start;

	if (segment.Length() == 0.0f)
	{
		return _start;
	}

	// 軸の始点から、点までのベクトル
	Vector3 toPoint{ _point - _start };

	// 軸の始点から、点までのベクトルを線に射影
	float shadow{ Vector3::Dot(toPoint, segment) / Vector3::Dot(segment, segment) };

	// 軸上にクランプ
	shadow = std::clamp(shadow, 0.0f, 1.0f);

	// 最近点を求める
	return _start + segment * shadow;
}

// 線分と点の距離
float MDMath::DistancePointToSegment(const Vector3& _point, const Vector3& _start, const Vector3& _end)
{
	Vector3 latestPoint{ ClosestPointOnSegment(_point,_start,_end) };

	return (latestPoint - _point).Length();
}

// 線分と線分の最近点
MDMath::ClosestPointsBetweenSegmentsResult MDMath::ClosestSegmentOnSegment(const Vector3& _startA, const Vector3& _endA, const Vector3& _startB, const Vector3& _endB)
{
    // 線分を作る
    const Vector3 segmentA{ _endA - _startA };
    const Vector3 segmentB{ _endB - _startB };
    // 始点距離
    const Vector3 r{ _startA - _startB };

    const float lengthA{ Vector3::Dot(segmentA, segmentA) };
    const float lengthB{ Vector3::Dot(segmentB, segmentB) };
    const float dotA{ Vector3::Dot(segmentA, r) };
    const float dotB{ Vector3::Dot(segmentB, r) };

    float s{ 0.0f };
    float t{ 0.0f };

    if (lengthA <= MathConstants::EPSILON &&
        lengthB <= MathConstants::EPSILON)
    {
        // 両方とも点
    }
    else if (lengthA <= MathConstants::EPSILON)
    {
        // Aだけ点
        t = std::clamp(dotB / lengthB, 0.0f, 1.0f);
    }
    else if (lengthB <= MathConstants::EPSILON)
    {
        // Bだけ点
        s = std::clamp(-dotA / lengthA, 0.0f, 1.0f);
    }
    else
    {
        // 両方とも線分
        /*
               最近点ということは、最近点同士を結んだベクトルとAとBの辺の内積は0になるはず
               dot(dotA A(s) - B(t)) = 0
               dot(dotB, A(s) - B(t)) = 0
               になるsとtを求める
               連立方程式として解いたら下のような式になる
        */
        const float dotAB{ Vector3::Dot(segmentA, segmentB) };
        const float denominator{ lengthA * lengthB - dotAB * dotAB };

        if (std::abs(denominator) > MathConstants::EPSILON)
        {
            s = std::clamp(
                (dotAB * dotB - dotA * lengthB) / denominator,
                0.0f,
                1.0f);
        }

        t = (dotAB * s + dotB) / lengthB;

        if (t < 0.0f)
        {
            t = 0.0f;
            s = std::clamp(-dotA / lengthA, 0.0f, 1.0f);
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
            s = std::clamp(
                (dotAB - dotA) / lengthA,
                0.0f,
                1.0f);
        }
    }

    return ClosestPointsBetweenSegmentsResult
    {
        _startA + segmentA * s,
        _startB + segmentB * t
    };
}

MDMath::ClosestPointsBetweenSegmentAndAABBResult MDMath::ClosestPointsBetweenSegmentAndAABB(
    const Vector3& _segmentStart, const Vector3& _segmentEnd,
    const Vector3& _boxPos, const Vector3& _boxSize)
{
    Vector3 halfSize{ _boxSize / 2.0f };

    Vector3 aabbMin{ _boxPos - halfSize };

    Vector3 aabbMax{ _boxPos + halfSize };

    // 線分の方向
    Vector3 direction{ _segmentEnd - _segmentStart };

    /*
        X、Y、Zを同じ処理で計算できるように、
        Vector3の各成分を配列へ取り出す。
    */
    float start[3]
    {
        _segmentStart.x,
        _segmentStart.y,
        _segmentStart.z
    };

    float dir[3]
    {
        direction.x,
        direction.y,
        direction.z
    };

    float minValue[3]
    {
        aabbMin.x,
        aabbMin.y,
        aabbMin.z
    };

    float maxValue[3]
    {
        aabbMax.x,
        aabbMax.y,
        aabbMax.z
    };

    /*
        線分上の点がAABBの面を通過するtを保存する。

        AABBには、
            Xの最小面・最大面
            Yの最小面・最大面
            Zの最小面・最大面

        の6面がある。

        線分の始点と終点を合わせると、
        必要なtは最大8個になる。
    */
    std::array<float, 8> breakpoints{};
    size_t breakpointCount{ 0 };

    // 線分の始点と終点
    breakpoints[breakpointCount++] = 0.0f;
    breakpoints[breakpointCount++] = 1.0f;

    /*
        P(t)の各成分は、

            P_axis(t) = start_axis + dir_axis * t

        AABBの面boundaryと交差するtは、

            start_axis + dir_axis * t = boundary

        これをtについて解いて、

            t = (boundary - start_axis) / dir_axis

        となる。
    */
    for (int axis{ 0 }; axis < 3; axis++)
    {
        // 0チェック
        if (std::abs(dir[axis]) <= MathConstants::EPSILON)
        {
            continue;
        }

        // 最小側の面を通過するt
        float minT{ (minValue[axis] - start[axis]) / dir[axis] };

        // 最大側の面を通過するt
        float maxT{ (maxValue[axis] - start[axis]) / dir[axis] };

        // 線分内にある境界だけを追加する
        if (minT > 0.0f && minT < 1.0f)
        {
            breakpoints[breakpointCount++] = minT;
        }

        if (maxT > 0.0f && maxT < 1.0f)
        {
            breakpoints[breakpointCount++] = maxT;
        }
    }

    // tの小さい順に並べる
    std::sort(breakpoints.begin(), breakpoints.begin() + breakpointCount);

    /*
        Boxの角を通る場合などは、複数軸で同じtが出る。

        ほぼ同じtを一つにまとめる。
    */
    size_t uniqueCount{ 1 };

    for (size_t i{ 1 }; i < breakpointCount; i++)
    {
        float previous{ breakpoints[uniqueCount - 1] };

        if (std::abs(breakpoints[i] - previous) > MathConstants::EPSILON)
        {
            breakpoints[uniqueCount++] = breakpoints[i];
        }
    }

    // 現在見つかっている最良の結果
    Vector3 bestPointOnSegment{ _segmentStart };

    Vector3 bestPointOnAABB{
        std::clamp(_segmentStart.x,aabbMin.x,aabbMax.x),
        std::clamp(_segmentStart.y,aabbMin.y,aabbMax.y),
        std::clamp(_segmentStart.z,aabbMin.z,aabbMax.z)
    };

    float bestParameter{ 0.0f };

    float bestDistanceSqr{ FLT_MAX };

    /*
        指定したtにおける、

            線分上の点
            AABB上の最近点
            その2点間の距離

        を調べる関数
    */
    auto Evaluate = [&](float _t)
        {
            // 線分上の点
            Vector3 pointOnSegment{ _segmentStart + direction * _t };

            // AABB内にクランプ
            Vector3 pointOnAABB{
                std::clamp(pointOnSegment.x,aabbMin.x,aabbMax.x),
                std::clamp(pointOnSegment.y,aabbMin.y,aabbMax.y),
                std::clamp(pointOnSegment.z,aabbMin.z,aabbMax.z)
            };

            float distanceSqr{ (pointOnSegment - pointOnAABB).LengthSqr() };

            if (distanceSqr < bestDistanceSqr)
            {
                bestDistanceSqr = distanceSqr;
                bestParameter = _t;
                bestPointOnSegment = pointOnSegment;
                bestPointOnAABB = pointOnAABB;
            }
        };

    // 全部で調べる
    for (size_t i{ 0 }; i < uniqueCount; i++)
    {
        Evaluate(breakpoints[i]);
    }

    /*
        breakpointsで区切られた各区間を調べる。

        同じ区間内では、線分上の点が各軸について、

            AABBの最小値より外側
            AABBの内側
            AABBの最大値より外側

        のどこにいるかが変化しない。

        そのため、その区間内の距離の2乗は、
        一つの二次関数として扱える。
    */
    for (size_t i{ 0 }; i + 1 < uniqueCount; i++)
    {
        float intervalStart{ breakpoints[i] };

        float intervalEnd{ breakpoints[i + 1] };

        if (intervalEnd - intervalStart <= MathConstants::EPSILON)
        {
            continue;
        }

        /*
            区間の中央を見ることで、
            この区間では各軸がAABBのどちら側にあるかを判定する。
        */
        float middle{ (intervalStart + intervalEnd) * 0.5f };

        /*
            距離の2乗を次の二次関数として扱う。

                F(t) = A*t*t + 2*B*t + C

            微分すると、

                F'(t) = 2*A*t + 2*B

            最小値ではF'(t) = 0なので、

                t = -B / A

            となる。
        */
        float quadraticA{ 0.0f };
        float linearB{ 0.0f };

        for (int axis{ 0 }; axis < 3; axis++)
        {
            float middlePosition{ start[axis] + dir[axis] * middle };

            float nearestBoundary{ 0.0f };
            bool outside{ false };

            if (middlePosition < minValue[axis])
            {
                // AABBの最小側にいる
                nearestBoundary = minValue[axis];
                outside = true;
            }
            else if (middlePosition > maxValue[axis])
            {
                // AABBの最大側にいる
                nearestBoundary = maxValue[axis];
                outside = true;
            }

            /*
                AABBの内側にいる軸の距離は0なので、
                距離の二次関数へ加える必要がない。
            */
            if (!outside)
            {
                continue;
            }

            /*
                この軸における距離は、

                    start + dir*t - boundary

                距離の2乗は、

                    (start + dir*t - boundary)^2

                展開すると、

                    dir^2 * t^2
                    + 2*dir*(start - boundary)*t
                    + (start - boundary)^2

                となる。

                quadraticAにはdir^2、
                linearBにはdir*(start - boundary)
                を加える。
            */
            quadraticA +=dir[axis] * dir[axis];

            linearB += dir[axis] * (start[axis] - nearestBoundary);
        }

        /*
            軸方向に距離が変化する場合は、
            二次関数の頂点を求める。

                t = -B / A
        */
        float closestParameter{ middle };

        if (quadraticA > MathConstants::EPSILON)
        {
            closestParameter = -linearB / quadraticA;

            /*
                二次関数の頂点が現在の区間外にある場合、
                区間内での最小値は端にある。

                そのため現在の区間へクランプする。
            */
            closestParameter = std::clamp(closestParameter, intervalStart, intervalEnd);
        }

        Evaluate(closestParameter);
    }

    /*
        距離がほぼ0なら、
        線分はAABBに接触または侵入している。
    */
    bool intersects{ bestDistanceSqr <= MathConstants::EPSILON * MathConstants::EPSILON };

    return ClosestPointsBetweenSegmentAndAABBResult
    {
        bestPointOnSegment,
        bestPointOnAABB,
        bestParameter,
        bestDistanceSqr,
        intersects
    };
}

MDMath::ClosestPointsBetweenSegmentAndOBBResult MDMath::ClosestPointsBetweenSegmentAndOBB(
    const Vector3& _segmentStart, const Vector3& _segmentEnd,
    const Vector3& _boxPos, const Quaternion _boxRot, const Vector3& _boxSize)
{
    Quaternion inverseRotation{ _boxRot.Conjugate() };
   
    /*
        ワールド座標からOBBローカル座標へ変換。

        1. OBB中心位置を引く
        2. OBBの逆回転を適用する
    */
    Vector3 localSegmentStart{ inverseRotation.Rotate(_segmentStart - _boxPos) };

    Vector3 localSegmentEnd{ inverseRotation.Rotate(_segmentEnd - _boxPos) };

    /*
        OBBローカル空間ではBoxの中心は原点で、
        回転のないAABBとして扱える。
    */
    ClosestPointsBetweenSegmentAndAABBResult result{ ClosestPointsBetweenSegmentAndAABB(localSegmentStart,localSegmentEnd,Vector3::ZERO,_boxSize) };

    /*
        AABB版から返された最近点はOBBローカル座標。

        OBBの回転を適用し、OBB中心位置を足して
        ワールド座標へ戻す。
    */
    result.pointSegment = _boxPos + _boxRot.Rotate(result.pointSegment);

    result.pointAABB = _boxPos + _boxRot.Rotate(result.pointAABB);

    /*
        回転と平行移動では距離が変わらないため、
        distanceSqrはそのままでよい。

        parameterOnSegmentとintersectsも
        座標変換の影響を受けない。
    */
    return ClosestPointsBetweenSegmentAndOBBResult
    {
        result.pointSegment,
        result.pointAABB,
        result.parameterOnSegment,
        result.distanceSqr,
        result.intersects
    };
}
