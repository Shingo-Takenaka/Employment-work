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
        VECTOR playerPos);

    // 攻撃の描画
    void Draw(
        VECTOR enemyPos,
        VECTOR playerPos);

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

    // 状態開始からの経過フレーム
    int m_timer;

    // 予測線の点滅
    bool m_isWarningVisible;

    // Warning画像
    int m_warningGraph;

    // Beam画像
    int m_beamGraph;

    //攻撃開始時に記録したプレイヤー座標
    VECTOR m_attackTargetPos;

private:

    // 2D画像として攻撃を描画
    void DrawAttackImage(
        int graph,
        VECTOR start,
        VECTOR target,
        float imageAspect);
};