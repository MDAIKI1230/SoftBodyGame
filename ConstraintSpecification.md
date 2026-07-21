# Constraint Specification

## 目的

拘束は、元の拘束タイプに依存せず、最終的に共通の Solver 用 Constraint Row に変換して解く。

この仕様では、次の2つを分けて扱う。

- Authoring Constraint: Scene や Component で設定しやすい拘束情報
- Runtime Constraint Row: Solver が元の拘束タイプを知らずに解くための1行拘束

## 基本方針

拘束は「オブジェクトの所有物」だけではなく、「複数の点・Body・Entity の関係」として扱う。

拘束コンポーネントは配置場所を固定しない。

- 片方の Object に直接付けてもよい
- 拘束専用 Entity に付けてもよい
- 複数拘束をまとめる Constraint Set Entity に付けてもよい

重要なのは、Solver に渡す前にすべて同じ Runtime Constraint Row へ変換すること。

```text
Point / Distance / Joint / Contact などの Authoring Constraint
        ↓
Runtime Constraint Row
        ↓
Generic Constraint Solver
```

## Endpoint

拘束が作用する点は Endpoint として表す。

```cpp
struct ConstraintEndpoint
{
    EntityID entity;
    Vector3 localPoint;
};
```

`entity` は対象 Object を表す。

`localPoint` はその Entity の Transform を基準にしたローカル座標上の拘束点。

必要なら、Component が付いている Entity 自身を暗黙の対象として扱えるようにする。

```cpp
// entity が invalid の場合、Component owner Entity を使う
```

これにより、単純な2体拘束は片方の Object に直接書ける。

```text
Object A
  PointConstraintComponent
    endpoints[0] = self local point
    endpoints[1] = Object B local point
```

一方で、拘束を独立した関係として扱いたい場合は、拘束専用 Entity に置ける。

```text
Joint_AB Entity
  PointConstraintComponent
    endpoints[0] = Object A local point
    endpoints[1] = Object B local point
```

## Point Constraint

Point Constraint は、複数の Endpoint が同じワールド位置になることを目指す拘束。

```cpp
struct PointConstraintComponent : public ComponentBase
{
    std::vector<ConstraintEndpoint> endpoints;

    float stiffness;
    float damping;
};
```

2点の場合:

```text
worldPointA == worldPointB
```

3点以上の場合:

```text
worldPoint0 == worldPoint1 == worldPoint2 == ...
```

ただし、Solver では多点拘束をそのまま解かず、複数の2点拘束に展開する。

最初の実装では代表点方式を使う。

```cpp
for (int i = 1; i < endpoints.size(); ++i)
{
    // endpoints[0] と endpoints[i] を同じ位置にする拘束を生成する
}
```

全組み合わせ方式は拘束が強くなりすぎる可能性があるため、最初は使わない。

## Runtime Constraint Row

Solver が扱う拘束は、1本のスカラー拘束を表す Constraint Row とする。

```cpp
struct Constraint
{
    uint32_t solverBodyAIndex;
    uint32_t solverBodyBIndex;

    Vector3 jacobianLinearA;
    Vector3 jacobianAngularA;
    Vector3 jacobianLinearB;
    Vector3 jacobianAngularB;

    float constraintError;
    float bias;

    float accumulatedLambda;
    float minLambda;
    float maxLambda;
};
```

この4つの `Vector3` は、1行のヤコビアンを分割して持ったもの。

```text
J = [ linearA, angularA, linearB, angularB ]
v = [ vA, omegaA, vB, omegaB ]

Jv =
dot(linearA,  vA) +
dot(angularA, omegaA) +
dot(linearB,  vB) +
dot(angularB, omegaB)
```

剛体2つを扱う拘束では、並進3自由度と回転3自由度を2体分見るため、合計12要素になる。

## Point Constraint から Row への変換

Point Constraint はベクトル拘束なので、Solver には X/Y/Z の3本の Row として渡す。

```text
C.x = 0
C.y = 0
C.z = 0
```

2つの Endpoint から Row を作る場合:

```cpp
Vector3 worldPointA = transformA.TransformPoint(localPointA);
Vector3 worldPointB = transformB.TransformPoint(localPointB);

Vector3 rA = worldPointA - bodyA.position;
Vector3 rB = worldPointB - bodyB.position;

Vector3 error = worldPointA - worldPointB;
```

各軸ごとに Row を作る。

```cpp
Vector3 axes[3]
{
    Vector3{ 1.0f, 0.0f, 0.0f },
    Vector3{ 0.0f, 1.0f, 0.0f },
    Vector3{ 0.0f, 0.0f, 1.0f },
};

for (int i = 0; i < 3; ++i)
{
    Vector3 axis = axes[i];

    row.jacobianLinearA  = axis;
    row.jacobianAngularA = Vector3::Cross(rA, axis);
    row.jacobianLinearB  = -axis;
    row.jacobianAngularB = -Vector3::Cross(rB, axis);

    row.constraintError = Vector3::Dot(error, axis);
}
```

`rA` と `rB` が作用点を表す。

Solver に渡る時点では、作用点の情報は `jacobianAngularA/B` に畳み込まれている。

そのため Generic Solver は、元の作用点や拘束タイプを知らなくてよい。

## Lambda の制限

拘束タイプによって Lambda の範囲を変える。

Point Constraint や Joint のように押す・引くの両方が必要な拘束:

```cpp
row.minLambda = -FLT_MAX;
row.maxLambda =  FLT_MAX;
```

Contact Constraint のように押す方向だけの拘束:

```cpp
row.minLambda = 0.0f;
row.maxLambda = FLT_MAX;
```

## 配置パターン

### Object に直接置く

単純な2体拘束に向く。

```text
Object A
  PointConstraintComponent
    endpoints[0] = self
    endpoints[1] = Object B
```

メリット:

- 設定が直感的
- 拘束専用 Entity の位置や Transform を考えなくてよい
- 2つの Object を自由に動かしながら拘束する用途に向く

注意:

- 両方の Object に同じ拘束を書かない
- 重複して Solver に渡すと拘束が2回解かれる

### 拘束専用 Entity に置く

拘束を独立した関係として扱いたい場合に向く。

```text
PointJoint Entity
  PointConstraintComponent
    endpoints[0] = Object A
    endpoints[1] = Object B
```

メリット:

- Object A / B のどちらにも偏らない
- 拘束だけを生成・削除しやすい
- Scene 上で Joint として管理しやすい

### Constraint Set として置く

大量の同種拘束をまとめたい場合に向く。

```cpp
struct PointConstraintSetComponent : public ComponentBase
{
    std::vector<std::vector<ConstraintEndpoint>> constraints;

    float stiffness;
    float damping;
};
```

例:

```text
ChainConstraintSet
  A-B
  B-C
  C-D
  D-E
```

メリット:

- 鎖、布、SoftBody の辺拘束などをまとめやすい
- 同じ stiffness / damping を共有しやすい
- 複数 Object に同じ拘束ルールを適用しやすい

## 他拘束への拡張

Endpoint の仕組みは、Point Constraint 以外にも使える。

- Distance Constraint: Endpoint 同士の距離を一定にする
- Spring Constraint: Endpoint 同士をバネでつなぐ
- Hinge Constraint: Endpoint と軸を使って回転自由度を制限する
- Slider Constraint: Endpoint と軸を使って移動自由度を制限する
- Contact Constraint: 衝突点を一時的な Endpoint として扱う

各拘束タイプは、Authoring 情報から Runtime Constraint Row を生成する責務を持つ。

Solver は Runtime Constraint Row だけを扱う。

## 実装順序

最初は次の順で実装する。

1. Runtime Constraint Row の構造を作る
2. Generic Constraint Solver を作る
3. PointConstraintComponent を作る
4. PointConstraintComponent から Row を3本生成する System を作る
5. 2点拘束を解く
6. endpoints が3点以上の場合、代表点方式で複数の2点拘束へ展開する
7. Distance / Spring / Joint へ拡張する

## 設計上のルール

- Authoring Component は設定しやすさを優先する
- Runtime Constraint Row は Solver の単純さを優先する
- 拘束コンポーネントの配置場所を固定しない
- Solver は元の拘束タイプを知らない
- 多点拘束は Solver 前に Row へ展開する
- 作用点は Row 生成時に必要
- Solver 実行時には作用点ではなく Angular Jacobian を使う
- Contact 以外の拘束は基本的に押す・引くの両方向を許可する
