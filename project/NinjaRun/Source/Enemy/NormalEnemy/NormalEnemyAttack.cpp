#include "NormalEnemyAttack.h"

#include <math.h>

namespace
{
    // Warning‚Ì“_–ÅŠÔŠu
    const int WARNING_BLINK_INTERVAL = 7;

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
    const float ATTACK_HEIGHT = 3.0f;
}

NormalEnemyAttack::NormalEnemyAttack()
{
    m_state = AttackState::NONE;

    m_isWarningVisible = false;

    m_blinkTimer = 0;

    m_warningGraph = -1;
    m_beamGraph = -1;

    m_attackTargetPos =
        VGet(0.0f, 0.0f, 0.0f);

    m_warningGraph =
        LoadGraph(
            "Data/Enemy/NormalEnemy/Warning2.png");

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
    VECTOR playerPos,
    int animationFrame)
{
    // SHOOT‚Ì2–‡–Ú
    if (animationFrame == 1)
    {
        // 2–‡–Ú‚É“ü‚Á‚½uŠÔ
        if (m_state != AttackState::WARNING)
        {
            // ‚±‚ÌuŠÔ‚ÌPlayerˆÊ’u‚ğ‹L˜^
            m_attackTargetPos =
                playerPos;

            // WarningŠJn
            m_state =
                AttackState::WARNING;

            m_isWarningVisible =
                true;

            m_blinkTimer =
                0;
        }
        else
        {
            // Warning“_–Å
            m_blinkTimer++;

            if (m_blinkTimer >=
                WARNING_BLINK_INTERVAL)
            {
                m_blinkTimer = 0;

                m_isWarningVisible =
                    !m_isWarningVisible;
            }
        }

        return;
    }

    // SHOOT‚Ì3–‡–Ú
    if (animationFrame == 2)
    {
        // BeamŠJn
        m_state =
            AttackState::BEAM;

        return;
    }

    // ‚»‚êˆÈŠO‚ÌƒtƒŒ[ƒ€
    m_state =
        AttackState::NONE;

    m_isWarningVisible =
        false;

    m_blinkTimer =
        0;
}

void NormalEnemyAttack::Draw(
    VECTOR enemyPos)
{
    // UŒ‚‚µ‚Ä‚¢‚È‚¢
    if (m_state == AttackState::NONE)
    {
        return;
    }

    // Warning‚Ì“_–Å’†
    if (m_state == AttackState::WARNING &&
        !m_isWarningVisible)
    {
        return;
    }

    // UŒ‚•ûŒü
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

    // UŒ‚ŠJnˆÊ’u
    VECTOR start =
        enemyPos;

    start.y +=
        ATTACK_HEIGHT;

    // g—p‚·‚é’l
    float attackLength;
    float imageAspect;
    int graph;

    if (m_state == AttackState::WARNING)
    {
        if (m_warningGraph == -1)
        {
            return;
        }

        attackLength =
            WARNING_LENGTH;

        imageAspect =
            WARNING_ASPECT;

        graph =
            m_warningGraph;
    }
    else
    {
        if (m_beamGraph == -1)
        {
            return;
        }

        attackLength =
            BEAM_LENGTH;

        imageAspect =
            BEAM_ASPECT;

        graph =
            m_beamGraph;
    }

    // UŒ‚I—¹ˆÊ’u
    VECTOR target =
        start;

    target.x +=
        dx * attackLength;

    target.z +=
        dz * attackLength;

    DrawAttackImage(
        graph,
        start,
        target,
        imageAspect);
}

void NormalEnemyAttack::Reset()
{
    m_state =
        AttackState::NONE;

    m_isWarningVisible =
        false;

    m_blinkTimer =
        0;
}

void NormalEnemyAttack::DrawAttackImage(
    int graph,
    VECTOR start,
    VECTOR target,
    float imageAspect)
{
    // XZ•½–Êã‚Å‚ÌUŒ‚•ûŒü
    float dx =
        target.x -
        start.x;

    float dz =
        target.z -
        start.z;

    float length =
        sqrtf(
            dx * dx +
            dz * dz);

    if (length <= 0.001f)
    {
        return;
    }

    // UŒ‚•ûŒü‚ğ³‹K‰»
    float dirX =
        dx / length;

    float dirZ =
        dz / length;

    // ‰æ‘œ‚Ì‚‚³
    float imageHeight =
        length * imageAspect;

    float halfHeight =
        imageHeight * 0.5f;

    // UŒ‚•ûŒü‚É‘Î‚µ‚Ä‰¡Œü‚«‚Ì•ûŒü
    // XZ•½–Êã‚Å90“x‰ñ“]‚³‚¹‚é
    VECTOR side =
        VGet(
            -dirZ,
            0.0f,
            dirX);

    // \šŒ^‚É‚·‚é‚½‚ß‚Ìc•ûŒü
    VECTOR up =
        VGet(
            0.0f,
            1.0f,
            0.0f);

    // 1–‡–Ú‚Ìƒ|ƒŠƒSƒ“
    // UŒ‚•ûŒü ~ ã•ûŒü
    VECTOR p1 =
        VGet(
            start.x,
            start.y,
            start.z);

    VECTOR p2 =
        VGet(
            target.x,
            target.y,
            target.z);

    VECTOR topStart =
        VAdd(
            p1,
            VScale(up, halfHeight));

    VECTOR bottomStart =
        VSub(
            p1,
            VScale(up, halfHeight));

    VECTOR topTarget =
        VAdd(
            p2,
            VScale(up, halfHeight));

    VECTOR bottomTarget =
        VSub(
            p2,
            VScale(up, halfHeight));

    // 2–‡–Ú‚Ìƒ|ƒŠƒSƒ“
    // UŒ‚•ûŒü ~ ‰¡•ûŒü
    VECTOR sideStartPlus =
        VAdd(
            p1,
            VScale(side, halfHeight));

    VECTOR sideStartMinus =
        VSub(
            p1,
            VScale(side, halfHeight));

    VECTOR sideTargetPlus =
        VAdd(
            p2,
            VScale(side, halfHeight));

    VECTOR sideTargetMinus =
        VSub(
            p2,
            VScale(side, halfHeight));

    // ƒ‰ƒCƒeƒBƒ“ƒO‚Ì‰e‹¿‚ğó‚¯‚È‚¢
    SetUseLighting(FALSE);

    // •\— ‚Ç‚¿‚ç‚©‚ç‚Å‚àŒ©‚¦‚é‚æ‚¤‚É‚·‚é
    SetDrawMode(DX_DRAWMODE_BILINEAR);

    // 1–‡–Ú
    VERTEX3D vertex1[4];

    vertex1[0].pos =
        topStart;
    vertex1[0].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex1[0].dif =
        GetColorU8(255, 255, 255, 255);
    vertex1[0].spc =
        GetColorU8(0, 0, 0, 0);
    vertex1[0].u =
        0.0f;
    vertex1[0].v =
        0.0f;

    vertex1[1].pos =
        topTarget;
    vertex1[1].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex1[1].dif =
        GetColorU8(255, 255, 255, 255);
    vertex1[1].spc =
        GetColorU8(0, 0, 0, 0);
    vertex1[1].u =
        1.0f;
    vertex1[1].v =
        0.0f;

    vertex1[2].pos =
        bottomTarget;
    vertex1[2].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex1[2].dif =
        GetColorU8(255, 255, 255, 255);
    vertex1[2].spc =
        GetColorU8(0, 0, 0, 0);
    vertex1[2].u =
        1.0f;
    vertex1[2].v =
        1.0f;

    vertex1[3].pos =
        bottomStart;
    vertex1[3].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex1[3].dif =
        GetColorU8(255, 255, 255, 255);
    vertex1[3].spc =
        GetColorU8(0, 0, 0, 0);
    vertex1[3].u =
        0.0f;
    vertex1[3].v =
        1.0f;

    DrawPolygon3D(
        vertex1,
        4,
        graph,
        TRUE);

    // 2–‡–Ú
    VERTEX3D vertex2[4];

    vertex2[0].pos =
        sideStartPlus;
    vertex2[0].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex2[0].dif =
        GetColorU8(255, 255, 255, 255);
    vertex2[0].spc =
        GetColorU8(0, 0, 0, 0);
    vertex2[0].u =
        0.0f;
    vertex2[0].v =
        0.0f;

    vertex2[1].pos =
        sideTargetPlus;
    vertex2[1].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex2[1].dif =
        GetColorU8(255, 255, 255, 255);
    vertex2[1].spc =
        GetColorU8(0, 0, 0, 0);
    vertex2[1].u =
        1.0f;
    vertex2[1].v =
        0.0f;

    vertex2[2].pos =
        sideTargetMinus;
    vertex2[2].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex2[2].dif =
        GetColorU8(255, 255, 255, 255);
    vertex2[2].spc =
        GetColorU8(0, 0, 0, 0);
    vertex2[2].u =
        1.0f;
    vertex2[2].v =
        1.0f;

    vertex2[3].pos =
        sideStartMinus;
    vertex2[3].norm =
        VGet(0.0f, 1.0f, 0.0f);
    vertex2[3].dif =
        GetColorU8(255, 255, 255, 255);
    vertex2[3].spc =
        GetColorU8(0, 0, 0, 0);
    vertex2[3].u =
        0.0f;
    vertex2[3].v =
        1.0f;

    DrawPolygon3D(
        vertex2,
        4,
        graph,
        TRUE);

    // ƒ‰ƒCƒeƒBƒ“ƒO‚ğŒ³‚É–ß‚·
    SetUseLighting(TRUE);
}

bool NormalEnemyAttack::IsAttacking() const
{
    return m_state == AttackState::BEAM;
}

VECTOR NormalEnemyAttack::GetAttackTargetPos() const
{
    return m_attackTargetPos;
}