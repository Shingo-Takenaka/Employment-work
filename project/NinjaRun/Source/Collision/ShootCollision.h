#pragma once

#include "DxLib.h"

class PlayerAttack;
class NormalEnemy;

class ShootCollision
{
public:

    ShootCollision();

    // Žè— Œ•‚ÆEnemy‚Ì“–‚½‚è”»’è
    static bool Check(
        const PlayerAttack& playerAttack,
        int shootIndex,
        NormalEnemy& enemy
    );
};