#include "NormalEnemyAttack.h"

#include <math.h>

namespace
{
    // UŒ‚ŠJn‚Ü‚Å‚Ì—\‘ªŠÔ
    const int WARNING_TIME = 90;

    // UŒ‚ŠÔ
    const int BEAM_TIME = 30;

    // —\‘ªü‚Ì“_–ÅŠÔŠu
    const int WARNING_BLINK_INTERVAL = 10;

    // UŒ‚ŠJn‹——£
    const float ATTACK_RANGE = 50.0f;

    // UŒ‚‚Ì’·‚³
    const float WARNING_LENGTH = 50.0f;
    const float BEAM_LENGTH = 50.0f;

    // Warning.png
    // 1366 ~ 13
    const float WARNING_ASPECT =
        13.0f / 1366.0f;

    // Beam.png
    // 1103 ~ 102
    const float BEAM_ASPECT =
        102.0f / 1103.0f;

    // UŒ‚‚Ì”­ËˆÊ’u
    const float ATTACK_HEIGHT = 0.0f;
}

NormalEnemyAttack::NormalEnemyAttack()
{
    m_state = AttackState::NONE;

    m_timer = 0;

    m_isWarningVisible = false;

    m_warningGraph = -1;
    m_beamGraph = -1;

    m_attackTargetPos =
        VGet(0.0f, 0.0f, 0.0f);

    // Warning‰æ‘œ
    m_warningGraph =
        LoadGraph(
            "Data/Enemy/NormalEnemy/Warning.png");

    // Beam‰æ‘œ
    m_beamGraph =
        LoadGraph(
            "Data/Enemy/NormalEnemy/Beam.png");
}

NormalEnemyAttack::~NormalEnemyAttack()
{
    if (m_warningGraph != -1)
    {
        DeleteGraph(m_warningGraph);
    }

    if (m_beamGraph != -1)
    {
        DeleteGraph(m_beamGraph);
    }
}

void NormalEnemyAttack::Update(
    VECTOR enemyPos,
    VECTOR playerPos)
{
    float dx =
        playerPos.x - enemyPos.x;

    float dz =
        playerPos.z - enemyPos.z;

    float distance =
        sqrtf(
            dx * dx +
            dz * dz);

    // ‘Ò‹@’†
    if (m_state == AttackState::NONE)
    {
        if (distance <= ATTACK_RANGE)
        {
            // UŒ‚ŠJn‚ÌƒvƒŒƒCƒ„[ˆÊ’u‚ğ•Û‘¶
            m_attackTargetPos =
                playerPos;

            m_state =
                AttackState::WARNING;

            m_timer = 0;

            m_isWarningVisible = true;
        }

        return;
    }

    // —\‘ª’†
    if (m_state == AttackState::WARNING)
    {
        m_timer++;

        // —\‘ªü‚ğ“_–Å
        if (m_timer %
            WARNING_BLINK_INTERVAL == 0)
        {
            m_isWarningVisible =
                !m_isWarningVisible;
        }

        // —\‘ªI—¹
        if (m_timer >= WARNING_TIME)
        {
            m_state =
                AttackState::BEAM;

            m_timer = 0;
        }

        return;
    }

    // UŒ‚’†
    if (m_state == AttackState::BEAM)
    {
        m_timer++;

        if (m_timer >= BEAM_TIME)
        {
            m_state =
                AttackState::NONE;

            m_timer = 0;
        }
    }
}

void NormalEnemyAttack::Draw(
    VECTOR enemyPos,
    VECTOR playerPos)
{
    // UŒ‚ŠJn‚É‹L˜^‚µ‚½ƒvƒŒƒCƒ„[ˆÊ’u‚ğg—p
    float dx =
        m_attackTargetPos.x -
        enemyPos.x;

    float dz =
        m_attackTargetPos.z -
        enemyPos.z;

    float length =
        sqrtf(
            dx * dx +
            dz * dz);

    if (length <= 0.001f)
    {
        return;
    }

    // UŒ‚•ûŒü‚ğ³‹K‰»
    dx /= length;
    dz /= length;

    // UŒ‚‚Ì”­ËˆÊ’u
    VECTOR start =
        enemyPos;

    start.y += ATTACK_HEIGHT;

    // —\‘ªü
    if (m_state == AttackState::WARNING)
    {
        if (!m_isWarningVisible)
        {
            return;
        }

        if (m_warningGraph == -1)
        {
            return;
        }

        VECTOR target =
            start;

        target.x +=
            dx * WARNING_LENGTH;

        target.z +=
            dz * WARNING_LENGTH;

        DrawAttackImage(
            m_warningGraph,
            start,
            target,
            WARNING_ASPECT);

        return;
    }

    // UŒ‚ƒr[ƒ€
    if (m_state == AttackState::BEAM)
    {
        if (m_beamGraph == -1)
        {
            return;
        }

        VECTOR target =
            start;

        target.x +=
            dx * BEAM_LENGTH;

        target.z +=
            dz * BEAM_LENGTH;

        DrawAttackImage(
            m_beamGraph,
            start,
            target,
            BEAM_ASPECT);
    }
}

void NormalEnemyAttack::DrawAttackImage(
    int graph,
    VECTOR start,
    VECTOR target,
    float imageAspect)
{
    float dx =
        target.x - start.x;

    float dz =
        target.z - start.z;

    float length =
        sqrtf(
            dx * dx +
            dz * dz);

    if (length <= 0.001f)
    {
        return;
    }

    // UŒ‚•ûŒü‚ÌŠp“x
    float angle =
        -atan2f(
            dz,
            dx);

    // ‰æ‘œ‚Ì‚‚³
    float imageHeight =
        length * imageAspect;

    float halfHeight =
        imageHeight * 0.5f;

    // UŒ‚‰æ‘œ‚Ì‰¡•
    float imageWidth =
        length;

    // Šp“xŒvZ
    float cosAngle =
        cosf(angle);

    float sinAngle =
        sinf(angle);

    // 2D‰æ‘œã‚ÌÀ•W‚ğ‰ñ“]
    auto RotatePoint =
        [cosAngle, sinAngle](
            float x,
            float y)
        {
            VECTOR result;

            result.x =
                x * cosAngle -
                y * sinAngle;

            result.y =
                x * sinAngle +
                y * cosAngle;

            result.z =
                0.0f;

            return result;
        };

    // ‰æ‘œ‚Ì¶’[‚ğEnemy‚Ì’†S‚É‚·‚é
    VECTOR topRight =
        RotatePoint(
            imageWidth,
            -halfHeight);

    VECTOR topLeft =
        RotatePoint(
            0.0f,
            -halfHeight);

    VECTOR bottomLeft =
        RotatePoint(
            0.0f,
            halfHeight);

    VECTOR bottomRight =
        RotatePoint(
            imageWidth,
            halfHeight);

    // start‚ğ‰æ‘œ‚ÌŠî€ˆÊ’u‚É‚·‚é
    DrawModiBillboard3D(
        start,

        topRight.x,
        topRight.y,

        topLeft.x,
        topLeft.y,

        bottomLeft.x,
        bottomLeft.y,

        bottomRight.x,
        bottomRight.y,

        graph,
        TRUE);
}