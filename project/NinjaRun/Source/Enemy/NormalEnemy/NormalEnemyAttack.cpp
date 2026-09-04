#include "NormalEnemyAttack.h"

#include <math.h>

namespace
{
    // ==============================
    // 攻撃設定
    // ==============================

    // 攻撃開始までの予測時間
    const int WARNING_TIME = 90;

    // 攻撃時間
    const int BEAM_TIME = 30;

    // 予測線の点滅間隔
    const int WARNING_BLINK_INTERVAL = 10;

    // 攻撃開始距離
    const float ATTACK_RANGE = 50.0f;

    // 予測線の長さ
    const float WARNING_LENGTH = 50.0f;

    // 攻撃ビームの長さ
    const float BEAM_LENGTH = 50.0f;

    // ==============================
    // 見た目設定
    // ==============================

    // 予測線の色
    const int WARNING_COLOR =
        GetColor(255, 220, 0);   // 黄色

    // 攻撃ビームの色
    const int BEAM_COLOR =
        GetColor(255, 80, 80);   // 赤色

    // 攻撃ビームの太さ
    // 数値を大きくすると太くなる
    const float BEAM_THICKNESS = 2.0f;

    // ビームを構成する線の本数
    const int BEAM_LINE_COUNT = 7;
}


NormalEnemyAttack::NormalEnemyAttack()
{
    m_state = AttackState::NONE;
    m_timer = 0;
    m_isWarningVisible = false;
}


void NormalEnemyAttack::Update(
    VECTOR enemyPos,
    VECTOR playerPos)
{
    float dx = playerPos.x - enemyPos.x;
    float dz = playerPos.z - enemyPos.z;

    float distance = sqrtf(dx * dx + dz * dz);

    // 攻撃範囲外なら何もしない
    if (m_state == AttackState::NONE)
    {
        if (distance <= ATTACK_RANGE)
        {
            m_state = AttackState::WARNING;
            m_timer = 0;
            m_isWarningVisible = true;
        }

        return;
    }

    // ==============================
    // 予測線
    // ==============================
    if (m_state == AttackState::WARNING)
    {
        m_timer++;

        // 点滅
        if (m_timer % WARNING_BLINK_INTERVAL == 0)
        {
            m_isWarningVisible =
                !m_isWarningVisible;
        }

        // 予測終了 → 攻撃
        if (m_timer >= WARNING_TIME)
        {
            m_state = AttackState::BEAM;
            m_timer = 0;
        }

        return;
    }

    // ==============================
    // 攻撃中
    // ==============================
    if (m_state == AttackState::BEAM)
    {
        m_timer++;

        if (m_timer >= BEAM_TIME)
        {
            m_state = AttackState::NONE;
            m_timer = 0;
        }
    }
}


void NormalEnemyAttack::Draw(
    VECTOR enemyPos,
    VECTOR playerPos)
{
    float dx = playerPos.x - enemyPos.x;
    float dz = playerPos.z - enemyPos.z;

    float length = sqrtf(dx * dx + dz * dz);

    if (length <= 0.001f)
        return;

    // XZ平面上の方向を正規化
    dx /= length;
    dz /= length;

    // 攻撃線の高さ
    const float attackHeight = 5.0f;

    VECTOR start = enemyPos;
    start.y += attackHeight;

    // ==============================
    // 予測線
    // ==============================
    if (m_state == AttackState::WARNING)
    {
        if (!m_isWarningVisible)
            return;

        VECTOR end = start;

        end.x += dx * WARNING_LENGTH;
        end.z += dz * WARNING_LENGTH;

        DrawLine3D(
            start,
            end,
            WARNING_COLOR
        );

        return;
    }

    // ==============================
    // 攻撃ビーム
    // ==============================
    if (m_state == AttackState::BEAM)
    {
        VECTOR end = start;

        end.x += dx * BEAM_LENGTH;
        end.z += dz * BEAM_LENGTH;

        // ビーム方向に対して垂直な方向
        float sideX = -dz;
        float sideZ = dx;

        // 複数の線を重ねて太く見せる
        for (int i = 0; i < BEAM_LINE_COUNT; i++)
        {
            float offset =
                (i - (BEAM_LINE_COUNT - 1) * 0.5f)
                * BEAM_THICKNESS;

            VECTOR beamStart = start;
            VECTOR beamEnd = end;

            beamStart.x += sideX * offset;
            beamStart.z += sideZ * offset;

            beamEnd.x += sideX * offset;
            beamEnd.z += sideZ * offset;

            DrawLine3D(
                beamStart,
                beamEnd,
                BEAM_COLOR
            );
        }

        return;
    }
}