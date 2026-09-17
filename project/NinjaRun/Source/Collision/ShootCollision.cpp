#include "ShootCollision.h"

#include "../Ninja/NinjaAttack.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"

#include <math.h>

ShootCollision::ShootCollision()
{
}

// Žè— Œ•‚ÆEnemy‚ª“–‚½‚Á‚Ä‚¢‚é‚©
bool ShootCollision::Check(
    const NinjaAttack& ninjaAttack,
    int shootIndex,
    NormalEnemy& enemy)
{
    if (enemy.IsDead())
    {
        return false;
    }

    VECTOR shootPos =
        ninjaAttack.GetShootPosition(shootIndex);

    VECTOR enemyPos =
        enemy.GetPosition();

    // Žè— Œ•‚Ì“–‚½‚è”»’è
    float shootWidth = 3.0f;
    float shootDepth = 6.0f;
    float shootHeight = 6.0f;

    float dx =
        fabsf(enemyPos.x - shootPos.x);

    if (dx > shootWidth)
    {
        return false;
    }

    float dz =
        fabsf(enemyPos.z - shootPos.z);

    if (dz > shootDepth)
    {
        return false;
    }

    float dy =
        fabsf(enemyPos.y - shootPos.y);

    if (dy > shootHeight)
    {
        return false;
    }

    return true;
}