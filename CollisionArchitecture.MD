# ECS物理エンジン設計案

## 目的

* ECSのデータ指向設計を維持する
* GJKの拡張性を最大限活かす
* コライダー追加時にGJK本体を変更しない
* BroadPhaseとNarrowPhaseを明確に分離する

---

# 全体構成

```text
Entity
 ↓
ColliderHandle
 ↓
BroadPhase
 ↓
CollisionPair
 ↓
Handle解決
 ↓
ShapeProxy生成
 ↓
GJK
 ↓
EPA
 ↓
Collision Result
```

---

# Collider Storage

各コライダーは型ごとの専用ストレージで管理する。

```cpp
SphereColliderStorage
BoxColliderStorage
CapsuleColliderStorage
ConvexHullStorage
```

例：

```cpp
struct SphereCollider
{
    Vector3 Center;
    float Radius;
};

class SphereColliderStorage
{
public:
    std::vector<SphereCollider> Colliders;
};
```

---

# ColliderHandle

コライダー実体への参照情報。

```cpp
struct ColliderHandle
{
    ColliderType Type;
    uint32_t Index;
};
```

## 役割

* ストレージ内の実体位置を特定
* BroadPhaseで利用
* NarrowPhaseで利用

---

# BroadPhase

BroadPhaseはコライダー形状を知らない。

必要なのはAABBのみ。

```cpp
struct BroadPhaseProxy
{
    ColliderHandle Handle;
    AABB Bounds;
};
```

---

# SAP

各ストレージからAABBを収集する。

```text
SphereStorage
BoxStorage
CapsuleStorage
        ↓
      AABB
        ↓
   SAP入力配列
```

BroadPhaseはSphereやBoxの違いを意識しない。

---

# CollisionPair

BroadPhaseの出力。

```cpp
struct CollisionPair
{
    ColliderHandle A;
    ColliderHandle B;
};
```

---

# NarrowPhase

CollisionPairから実際の衝突判定を行う。

```text
CollisionPair
 ↓
Handle解決
 ↓
ShapeProxy生成
 ↓
GJK
```

---

# ShapeProxy

GJK用の抽象形状。

## 目的

GJKがSphereやBoxなどの具体的な型を知らなくても動作するようにする。

```cpp
struct ShapeProxy
{
    Vector3 (*Support)(void* Data, const Vector3& Dir);

    void* Data;
};
```

---

# ShapeProxyは実データを持たない

重要：

```text
ShapeProxyはコライダー情報を保持しない
```

例えば以下は行わない。

```cpp
struct ShapeProxy
{
    Vector3 Center;
    float Radius;
};
```

理由：

* 毎フレームコピーが発生する
* Storageとの同期が必要になる
* ECSの利点が失われる

---

# 実データの所有者

実際の数値は全てStorage側に存在する。

```text
SphereStorage
 └ SphereCollider

BoxStorage
 └ BoxCollider

CapsuleStorage
 └ CapsuleCollider
```

---

# ShapeProxy生成

Sphereの場合

```cpp
SphereCollider* sphere =
    sphereStorage.Get(index);

ShapeProxy proxy;

proxy.Support = SphereSupport;
proxy.Data = sphere;
```

Boxの場合

```cpp
BoxCollider* box =
    boxStorage.Get(index);

ShapeProxy proxy;

proxy.Support = BoxSupport;
proxy.Data = box;
```

---

# Support関数

Sphere

```cpp
Vector3 SphereSupport(
    void* data,
    const Vector3& dir)
{
    auto* sphere =
        static_cast<SphereCollider*>(data);

    return sphere->Center +
           Normalize(dir) * sphere->Radius;
}
```

Box

```cpp
Vector3 BoxSupport(
    void* data,
    const Vector3& dir);
```

Capsule

```cpp
Vector3 CapsuleSupport(
    void* data,
    const Vector3& dir);
```

---

# データ参照構造

```text
ShapeProxy
    │
    ▼
SphereCollider*
    │
    ▼
SphereStorage内データ
```

つまり

```text
Storage
 ↑
参照
 ↑
ShapeProxy
```

の関係になる。

---

# GJK

GJKはShapeProxyのみを扱う。

```cpp
bool GJK(
    const ShapeProxy& A,
    const ShapeProxy& B);
```

---

# GJKが知る情報

```cpp
Support(dir)
```

のみ。

---

# GJKが知らない情報

```text
Sphere
Box
Capsule
ConvexHull
Storage構造
Entity
```

---

# GJKの責務

* Minkowski Difference構築
* Support呼び出し
* Simplex更新
* 原点包含判定

---

# EPA

GJK衝突時のみ実行。

```cpp
EPAResult EPA(
    const ShapeProxy& A,
    const ShapeProxy& B,
    const Simplex& Simplex);
```

取得情報：

```text
接触法線
貫通深度
接触点
```

---

# 新規コライダー追加

例：Cylinder

追加するもの

```cpp
CylinderCollider
CylinderStorage
CylinderSupport()
```

変更不要

```text
GJK
EPA
Simplex
BroadPhase
```

---

# CollisionSystemの責務

```text
CollisionPair取得
 ↓
Handle解決
 ↓
ShapeProxy生成
 ↓
GJK実行
 ↓
EPA実行
 ↓
結果出力
```

---

# 設計思想

ECS層

```text
Storage
Handle
Pair
```

を管理する。

GJK層

```text
Support関数のみ利用
```

する。

そのためGJKは完全な形状非依存アルゴリズムとして実装できる。

---

# 最終構造

```text
SphereStorage
BoxStorage
CapsuleStorage
        ↓
   ColliderHandle
        ↓
        SAP
        ↓
 CollisionPair
        ↓
 Handle解決
        ↓
  ShapeProxy
        ↓
       GJK
        ↓
       EPA
        ↓
 CollisionResult
```

## メリット

* ECSとの相性が良い
* Storageが唯一のデータ所有者
* GJK本体を変更せず形状追加可能
* BroadPhaseとNarrowPhaseの責務が明確
* ShapeProxyが軽量
* データコピーが発生しない
* Support関数のみで形状を抽象化できる

```
```
