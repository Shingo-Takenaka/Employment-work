#pragma once

#include "DxLib.h"

class Ninja;
class NormalEnemy;

class TouchEnemy
{
public:

    // 接触判定
    static bool Check(
        const Ninja& ninja,
        const NormalEnemy& enemy
    );

    // ノックバック
    static void Apply(
        Ninja& ninja,
        const NormalEnemy& enemy
    );

    // X方向の判定範囲
    static float GetTouchWidth();

    // Z方向の判定範囲
    static float GetTouchDepth();
};