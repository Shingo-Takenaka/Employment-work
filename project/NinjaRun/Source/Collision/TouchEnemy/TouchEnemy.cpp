#include "TouchEnemy.h"
#include "../../Ninja/Ninja.h"
#include "../../Enemy/EnemyBase/EnemyBase.h"
#include "../CollisionSetting.h"
#include <math.h>

// ノックバックの強さ
const float KNOCKBACK_STRENGTH = 0.5f;

// ノックバックして動けなくなる時間（秒）
const float KNOCKBACK_DURATION = 0.5f;

//判定範囲
float TouchEnemy::GetTouchWidth()
{
    return CollisionSettings::TOUCH_ENEMY_WIDTH;
}
float TouchEnemy::GetTouchDepth()
{
    return CollisionSettings::TOUCH_ENEMY_DEPTH;
}
float TouchEnemy::GetTouchHeight()
{
    return CollisionSettings::TOUCH_ENEMY_HEIGHT;
}

// 接触判定
bool TouchEnemy::Check(const Ninja& ninja, const EnemyBase& enemy)
{
    // 現在のEnemyの座標を基準にする
    const EnemyBase& enemyRef = enemy;

    VECTOR enemyPos = enemyRef.GetPosition();

    // Playerの現在座標
    VECTOR ninjaPos = ninja.GetPosition();

    // EnemyからPlayerまでの距離
    float dx = fabsf(ninjaPos.x - enemyPos.x);
    float dy = fabsf(ninjaPos.y - enemyPos.y);
    float dz = fabsf(ninjaPos.z - enemyPos.z);

    // Enemyを中心にした判定範囲
    const float width = GetTouchWidth();
    const float height = GetTouchHeight();
    const float depth = GetTouchDepth();

    return
        dx <= width &&
        dy <= height &&
        dz <= depth;
}

// ノックバック処理
void TouchEnemy::Apply(Ninja& ninja,const EnemyBase& enemy)
{
    // 現在のEnemyの座標を基準にする
    VECTOR enemyPos = enemy.GetPosition();

    VECTOR ninjaPos = ninja.GetPosition();

    // EnemyからPlayerへ向かう方向
    VECTOR direction = VSub(ninjaPos, enemyPos);

    // X・Z方向だけ使用する
    direction.y = 0.0f;

    // EnemyからPlayerまでの距離
    float length = sqrtf
    (
        direction.x * direction.x + direction.z * direction.z
    );

    // 同じ位置だった場合
    if (length <= 0.001f)
    {
        direction = VGet(1.0f, 0.0f, 0.0f);
    }
    else
    {
        // 正規化
        direction.x /= length;
        direction.z /= length;
    }

    ninja.ApplyKnockback(direction,KNOCKBACK_STRENGTH,KNOCKBACK_DURATION);
}