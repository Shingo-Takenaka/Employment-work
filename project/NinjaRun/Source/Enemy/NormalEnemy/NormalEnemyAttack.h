#pragma once

#include "DxLib.h"

class NormalEnemyAttack
{
public:

    NormalEnemyAttack();

    ~NormalEnemyAttack();

    // 攻撃処理の更新
    void Update(
        VECTOR enemyPos,
        VECTOR playerPos,
        int animationFrame);

    // 攻撃の描画
    void Draw(
        VECTOR enemyPos);

    // 攻撃状態をリセット
    void Reset();

private:

    // 攻撃状態
    enum class AttackState
    {
        NONE,
        WARNING,
        BEAM
    };

private:

    // 現在の攻撃状態
    AttackState m_state;

    // Warningの点滅
    bool m_isWarningVisible;

    // 点滅タイマー
    int m_blinkTimer;

    // Warning画像
    int m_warningGraph;

    // Beam画像
    int m_beamGraph;

    // Warning開始時に記録したPlayer座標
    VECTOR m_attackTargetPos;

private:

    // 3D空間上に攻撃画像を描画
    void DrawAttackImage(
        int graph,
        VECTOR start,
        VECTOR target,
        float width);
};