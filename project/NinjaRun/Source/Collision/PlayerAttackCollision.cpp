
#include "PlayerAttackCollision.h"

#include "../Ninja/Ninja.h"
#include "../Enemy/EnemyBase/EnemyBase.h"

#include <math.h>

PlayerAttackCollision::PlayerAttackCollision()
{
}

// Player‚ÌUŒ‚‚ÆEnemy‚ª“–‚½‚Á‚Ä‚¢‚é‚©
bool PlayerAttackCollision::Check(
    const Ninja& ninja,
    const EnemyBase& enemy)
{
    // UŒ‚’†‚Å‚È‚¯‚ê‚Î“–‚½‚è”»’è‚È‚µ
    if (!ninja.IsSlash())
    {
        return false;
    }

    VECTOR ninjaPos =
        ninja.GetPosition();

    VECTOR enemyPos =
        enemy.GetPosition();

    // PlayerAttack‚©‚çUŒ‚”ÍˆÍ‚ğæ“¾
    float attackWidth =
        m_playerAttack.GetAttackWidth();

    float attackDepth =
        m_playerAttack.GetAttackDepth();

    float attackHeight =
        m_playerAttack.GetAttackHeight();

    // X•ûŒü
    float minX;
    float maxX;

    if (!ninja.IsReverseX())
    {
        // ‰EŒü‚«
        minX = ninjaPos.x;
        maxX = ninjaPos.x + attackWidth;
    }
    else
    {
        // ¶Œü‚«
        minX = ninjaPos.x - attackWidth;
        maxX = ninjaPos.x;
    }

    // X•ûŒü‚Ì”»’è
    if (enemyPos.x < minX ||
        enemyPos.x > maxX)
    {
        return false;
    }

    // Z•ûŒü
    float dz =
        fabsf(enemyPos.z - ninjaPos.z);

    if (dz > attackDepth)
    {
        return false;
    }

    // Y•ûŒü
    float attackCenterY =
        ninjaPos.y - 5.0f;

    float dy =
        fabsf(enemyPos.y - attackCenterY);

    if (dy > attackHeight)
    {
        return false;
    }

    return true;
}