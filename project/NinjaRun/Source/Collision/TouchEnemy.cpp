#include "TouchEnemy.h"

#include "../Ninja/Ninja.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"

#include <math.h>

// ノックバックの強さ
const float KNOCKBACK_STRENGTH = 0.5f;

// ノックバックして動けなくなる時間（秒）
const float KNOCKBACK_DURATION = 0.5f;

// X方向の判定範囲
float TouchEnemy::GetTouchWidth()
{
    return 8.0f * 0.6f;
}

// Z方向の判定範囲
float TouchEnemy::GetTouchDepth()
{
    return 4.0f;
}

// Y方向の判定範囲
float TouchEnemy::GetTouchHeight()
{
    return 10.0f;
}

bool TouchEnemy::Check(
    const Ninja& ninja,
    const NormalEnemy& enemy)
{
    // 現在のEnemyの座標を基準にする
    VECTOR enemyPos =
        enemy.GetPosition();

    // Playerの現在座標
    VECTOR ninjaPos =
        ninja.GetPosition();

    // EnemyからPlayerまでのX方向の距離
    float dx =
        fabsf(ninjaPos.x - enemyPos.x);

    // EnemyからPlayerまでのY方向の距離
    float dy =
        fabsf(ninjaPos.y - enemyPos.y);

    // EnemyからPlayerまでのZ方向の距離
    float dz =
        fabsf(ninjaPos.z - enemyPos.z);

    // Enemyを中心にしたX方向の判定範囲
    const float width =
        GetTouchWidth();

    // Enemyを中心にしたY方向の判定範囲
    const float height =
        GetTouchHeight();

    // Enemyを中心にしたZ方向の判定範囲
    const float depth =
        GetTouchDepth();

    return
        dx <= width &&
        dy <= height &&
        dz <= depth;
}

void TouchEnemy::Apply(
    Ninja& ninja,
    const NormalEnemy& enemy)
{
    // 現在のEnemyの座標を基準にする
    VECTOR enemyPos =
        enemy.GetPosition();

    VECTOR ninjaPos =
        ninja.GetPosition();

    // EnemyからPlayerへ向かう方向
    VECTOR direction =
        VSub(ninjaPos, enemyPos);

    // X・Z方向だけ使用する
    direction.y = 0.0f;

    // EnemyからPlayerまでの距離
    float length =
        sqrtf(
            direction.x * direction.x +
            direction.z * direction.z);

    // 同じ位置だった場合
    if (length <= 0.001f)
    {
        direction =
            VGet(1.0f, 0.0f, 0.0f);
    }
    else
    {
        // 正規化
        direction.x /= length;
        direction.z /= length;
    }

    ninja.ApplyKnockback(
        direction,
        KNOCKBACK_STRENGTH,
        KNOCKBACK_DURATION);
}