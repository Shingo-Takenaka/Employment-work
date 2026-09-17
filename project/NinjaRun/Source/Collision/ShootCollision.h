#pragma once

#include "DxLib.h"

class NinjaAttack;
class NormalEnemy;

class ShootCollision
{
public:

    ShootCollision();

    // Žè— Œ•‚ÆEnemy‚Ì“–‚½‚è”»’è
    static bool Check(
        const NinjaAttack& ninjaAttack,
        int shootIndex,
        NormalEnemy& enemy);
};