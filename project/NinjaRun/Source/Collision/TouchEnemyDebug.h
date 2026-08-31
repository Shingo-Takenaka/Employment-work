#pragma once

#include "DxLib.h"

class Ninja;
class NormalEnemy;

class TouchEnemyDebug
{
public:

    // Enemy‚ÆNinja‚Ì“–‚½‚è”»’è”ÍˆÍ‚ð•`‰æ
    static void Draw(
        const Ninja& ninja,
        const NormalEnemy& enemy
    );
};