#include "NormalEnemyAttack.h"

#include <math.h>

namespace
{
    // Warningの点滅間隔
    const int WARNING_BLINK_INTERVAL = 10;

    // 攻撃の長さ
    const float WARNING_LENGTH = 50.0f;
    const float BEAM_LENGTH = 50.0f;

    // Warning.png
    // 1366 × 13
    const float WARNING_ASPECT =
        13.0f / 1366.0f;

    // Beam.png
    // 1103 × 102
    const float BEAM_ASPECT =
        102.0f / 1103.0f;

    // 攻撃の発射位置
    const float ATTACK_HEIGHT = 5.0f;
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
    // SHOOTの2枚目
    if (animationFrame == 1)
    {
        // 2枚目に入った瞬間
        if (m_state != AttackState::WARNING)
        {
            // この瞬間のPlayer位置を記録
            m_attackTargetPos =
                playerPos;

            // Warning開始
            m_state =
                AttackState::WARNING;

            m_isWarningVisible =
                true;

            m_blinkTimer =
                0;
        }
        else
        {
            // Warning点滅
            m_blinkTimer++;

            if (m_blinkTimer >=
                WARNING_BLINK_INTERVAL)
            {
                m_blinkTimer =
                    0;

                m_isWarningVisible =
                    !m_isWarningVisible;
            }
        }

        return;
    }

    // SHOOTの3枚目
    if (animationFrame == 2)
    {
        // Beam開始
        m_state =
            AttackState::BEAM;

        return;
    }

    // それ以外のフレーム
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
    // 攻撃していない
    if (m_state == AttackState::NONE)
    {
        return;
    }

    // Warningの点滅中
    if (m_state == AttackState::WARNING &&
        !m_isWarningVisible)
    {
        return;
    }

    // 攻撃方向
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

    // 攻撃方向を正規化
    dx /= length;
    dz /= length;

    // 攻撃開始位置
    VECTOR start =
        enemyPos;

    start.y +=
        ATTACK_HEIGHT;

    // 使用する値
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

    // 攻撃終了位置
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
    // XZ平面上での方向
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

    // 攻撃方向を正規化
    float dirX =
        dx / length;

    float dirZ =
        dz / length;

    // 画像の高さ
    float imageHeight =
        length * imageAspect;

    float halfHeight =
        imageHeight * 0.5f;

    // 画像の長さ
    float imageWidth =
        length;

    // XZ方向から角度を計算
    float angle =
        atan2f(
            dirZ,
            dirX);

    float cosAngle =
        cosf(angle);

    float sinAngle =
        sinf(angle);

    // DrawModiBillboard3Dのローカル座標を回転する
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

    // startを画像の左端中央として扱う
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

    // ライティングの影響を受けない
    SetUseLighting(FALSE);

    // 以前描画できていたBillboard描画を使用
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

    SetUseLighting(TRUE);
}