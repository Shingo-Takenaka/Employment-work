#pragma once

#include "DxLib.h"

enum class AttackState
{
    NONE,
    WARNING,
    BEAM
};

class NormalEnemyAttack
{
public:

    NormalEnemyAttack();
    ~NormalEnemyAttack();

    void Update(
        VECTOR enemyPos,
        VECTOR playerPos,
        int animationFrame);

    void Draw(
        VECTOR enemyPos);

    void Reset();

    // UŒ‚’†‚©
    bool IsAttacking() const;

    // UŒ‚‘ÎÛˆÊ’u‚ğæ“¾
    VECTOR GetAttackTargetPos() const;

private:

    void DrawAttackImage(
        int graph,
        VECTOR start,
        VECTOR target,
        float imageAspect);

private:

    AttackState m_state;

    bool m_isWarningVisible;

    int m_blinkTimer;

    int m_warningGraph;
    int m_beamGraph;

    VECTOR m_attackTargetPos;
};