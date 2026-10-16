#include "ShootCollision.h"
#include "../../Ninja/NinjaAttack.h"
#include "../../Enemy/EnemyBase/EnemyBase.h"
#include "../CollisionSetting.h"

#include <math.h>

ShootCollision::ShootCollision()
{
}

bool ShootCollision::Check(
    const NinjaAttack& ninjaAttack,
    int shootIndex,
    const EnemyBase& enemy)
{
    if (enemy.IsDead())
    {
        return false;
    }

    VECTOR shootPos = ninjaAttack.GetShootPosition(shootIndex);
    VECTOR enemyPos = enemy.GetPosition();

    float shootWidth = CollisionSettings::SHURIKEN_WIDTH;
    float shootDepth = CollisionSettings::SHURIKEN_DEPTH;
    float shootHeight = CollisionSettings::SHURIKEN_HEIGHT;

    float dx = fabsf(enemyPos.x - shootPos.x);

    if (dx > shootWidth)
    {
        return false;
    }

    float dz = fabsf(enemyPos.z - shootPos.z);

    if (dz > shootDepth)
    {
        return false;
    }

    float dy = fabsf(enemyPos.y - shootPos.y);

    if (dy > shootHeight)
    {
        return false;
    }

    return true;
}