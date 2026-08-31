#pragma once

#include "DxLib.h"

class Ninja;
class NormalEnemy;

class TouchEnemy
{
public:

    // Ninja‚ÆEnemy‚ªÚG‚µ‚Ä‚¢‚é‚©
    static bool Check(
        const Ninja& ninja,
        const NormalEnemy& enemy
    );

    // X•ûŒü‚Ì”»’è”ÍˆÍ‚ğæ“¾
    static float GetTouchWidth();

    // Z•ûŒü‚Ì”»’è”ÍˆÍ‚ğæ“¾
    static float GetTouchDepth();
};