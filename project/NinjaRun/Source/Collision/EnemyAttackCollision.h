#pragma once

#include "DxLib.h"

class Ninja;

class EnemyAttackCollision
{
public:

    // Enemy攻撃とPlayerの当たり判定
    static bool Check(
        const Ninja& ninja,
        VECTOR start,
        VECTOR target);

    // 攻撃範囲のデバッグ表示
    static void DrawDebug(
        VECTOR start,
        VECTOR target);
};