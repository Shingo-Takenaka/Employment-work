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
    VECTOR ninjaPos =
        ninja.GetPosition();

    VECTOR enemyPos =
        enemy.GetPosition();

    // X方向
    float dx =
        fabsf(ninjaPos.x - enemyPos.x);

    // Y方向
    float dy =
        fabsf(ninjaPos.y - enemyPos.y);

    // Z方向
    float dz =
        fabsf(ninjaPos.z - enemyPos.z);

    // X方向の判定
    float width =
        GetTouchWidth() * 2.0f;

    // Y方向の判定
    float height =
        GetTouchHeight();

    // Z方向の判定
    float depth =
        GetTouchDepth() * 2.0f;

    return
        dx <= width &&
        dy <= height &&
        dz <= depth;
}

void TouchEnemy::Apply(
    Ninja& ninja,
    const NormalEnemy& enemy)
{
    VECTOR ninjaPos =
        ninja.GetPosition();

    VECTOR enemyPos =
        enemy.GetPosition();

    // Enemy → Ninjaの方向
    VECTOR direction =
        VSub(ninjaPos, enemyPos);

    // X・Zだけ使用
    direction.y = 0.0f;

    // 長さ
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