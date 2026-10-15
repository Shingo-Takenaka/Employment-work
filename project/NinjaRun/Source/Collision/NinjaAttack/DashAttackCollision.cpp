#include "DashAttackCollision.h"

#include "../../Ninja/Ninja.h"
#include "../../Enemy/EnemyBase/EnemyBase.h"

#include <math.h>

DashAttackCollision::DashAttackCollision()
{
    m_attackWidth = 15.0f;
    m_attackDepth = 6.0f;
    m_attackHeight = 5.0f;
}

void DashAttackCollision::Update(const Ninja& ninja)
{
    if (!ninja.IsDashCut())
    {
        m_hitEnemies.clear();
    }
}

bool DashAttackCollision::Check(
    const Ninja& ninja,
    const EnemyBase& enemy)
{
    if (!ninja.IsDashCut() || enemy.IsDead())
    {
        return false;
    }

    for (const EnemyBase* hitEnemy : m_hitEnemies)
    {
        if (hitEnemy == &enemy)
        {
            return false;
        }
    }

    VECTOR ninjaPos = ninja.GetPosition();
    VECTOR enemyPos = enemy.GetPosition();

    float directionX = ninja.GetDashCutDirectionX();
    float directionZ = ninja.GetDashCutDirectionZ();

    float dx = enemyPos.x - ninjaPos.x;
    float dz = enemyPos.z - ninjaPos.z;

    float forwardDistance =
        dx * directionX + dz * directionZ;

    float sideDistance =
        fabsf(dx * directionZ - dz * directionX);

    if (forwardDistance < 0.0f ||
        forwardDistance > m_attackWidth)
    {
        return false;
    }

    if (sideDistance > m_attackDepth)
    {
        return false;
    }

    float attackCenterY = ninjaPos.y - 5.0f;
    float dy = fabsf(enemyPos.y - attackCenterY);

    if (dy > m_attackHeight)
    {
        return false;
    }

    m_hitEnemies.push_back(&enemy);

    return true;
}