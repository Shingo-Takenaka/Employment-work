#include "AttackCollision.h"
#include "../../Ninja/Ninja.h"
#include "../../Enemy/EnemyBase/EnemyBase.h"
#include "../CollisionSetting.h"

#include <math.h>

PlayerAttackCollision::PlayerAttackCollision()
    : m_wasSlashing(false)
{
}

void PlayerAttackCollision::Update(const Ninja& ninja)
{
    bool isSlashing = ninja.IsSlash();

    // 攻撃終了時に命中履歴をリセット
    if (!isSlashing && m_wasSlashing)
    {
        m_hitEnemies.clear();
    }

    m_wasSlashing = isSlashing;
}

bool PlayerAttackCollision::Check(
    const Ninja& ninja,
    const EnemyBase& enemy)
{
    // 攻撃中でなければ判定しない
    if (!ninja.IsSlash() || enemy.IsDead())
    {
        return false;
    }

    // すでに命中したEnemyには再度当てない
    for (const EnemyBase* hitEnemy : m_hitEnemies)
    {
        if (hitEnemy == &enemy)
        {
            return false;
        }
    }

    VECTOR ninjaPos = ninja.GetPosition();
    VECTOR enemyPos = enemy.GetPosition();

    float attackWidth =
        CollisionSettings::PLAYER_ATTACK_WIDTH;

    float attackDepth =
        CollisionSettings::PLAYER_ATTACK_DEPTH;

    float attackHeight =
        CollisionSettings::PLAYER_ATTACK_HEIGHT;

    float minX;
    float maxX;

    if (!ninja.IsReverseX())
    {
        minX = ninjaPos.x;
        maxX = ninjaPos.x + attackWidth;
    }
    else
    {
        minX = ninjaPos.x - attackWidth;
        maxX = ninjaPos.x;
    }

    // X方向
    if (enemyPos.x < minX || enemyPos.x > maxX)
    {
        return false;
    }

    // Z方向
    float dz = fabsf(enemyPos.z - ninjaPos.z);

    if (dz > attackDepth)
    {
        return false;
    }

    // Y方向
    float attackCenterY =
        ninjaPos.y +
        CollisionSettings::PLAYER_ATTACK_CENTER_Y_OFFSET;

    float dy = fabsf(enemyPos.y - attackCenterY);

    if (dy > attackHeight)
    {
        return false;
    }

    // 命中したEnemyを記録
    m_hitEnemies.push_back(&enemy);

    return true;
}
