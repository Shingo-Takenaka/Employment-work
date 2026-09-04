#pragma once

#include "DxLib.h"

class NormalEnemyAttack
{
public:

    NormalEnemyAttack();

    // 攻撃処理の更新
    void Update(
        VECTOR enemyPos,
        VECTOR playerPos);

    // 攻撃の描画
    void Draw(
        VECTOR enemyPos,
        VECTOR playerPos);

private:

    // 攻撃状態
    enum class AttackState
    {
        NONE,       // 攻撃していない
        WARNING,    // 予測線
        BEAM        // ビーム
    };

private:

    AttackState m_state;

    // 状態開始からの経過フレーム
    int m_timer;

    // 予測線の点滅用
    bool m_isWarningVisible;
};