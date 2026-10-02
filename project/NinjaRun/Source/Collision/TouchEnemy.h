#pragma once

#include "DxLib.h"

class Ninja;
class EnemyBase;

class TouchEnemy
{
public:
    // 接触判定
    static bool Check(
        const Ninja& ninja,
        const EnemyBase& enemy);

    // ノックバック処理
    static void Apply(
        Ninja& ninja,
        const EnemyBase& enemy);

    // X方向の判定範囲
    static float GetTouchWidth();

    // Z方向の判定範囲
    static float GetTouchDepth();

    // Y方向の判定範囲
    static float GetTouchHeight();
};