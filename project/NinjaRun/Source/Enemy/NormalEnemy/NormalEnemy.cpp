#include "NormalEnemy.h"

#include "../../Ninja/Ninja.h"

#include <math.h>

NormalEnemy::NormalEnemy()
    : EnemyBase(1)
{
    m_pos = VGet(40.0f, 0.0f, 50.0f);

    // 待機時は右向き
    m_isReverseX = false;

    // アニメーション読み込み
    m_animation.LoadAnimations();
}

NormalEnemy::~NormalEnemy()
{
}

// 更新
void NormalEnemy::Update(const Ninja& ninja)
{
    // 死亡していたら更新しない
    if (m_isDead)
    {
        return;
    }

    // Playerの座標を保存
    m_playerPos = ninja.GetPosition();

    // Playerの座標
    VECTOR ninjaPos =
        ninja.GetPosition();

    // X・Z方向の距離
    float dx =
        ninjaPos.x - m_pos.x;

    float dz =
        ninjaPos.z - m_pos.z;

    float distance =
        sqrtf(
            dx * dx +
            dz * dz);

    // 射撃判定
    const float shootRange = 50.0f;

    if (distance <= shootRange)
    {
        // 射撃アニメーション
        m_animation.SetAnimation(
            NormalEnemyAnim::SHOOT);

        if (ninjaPos.x < m_pos.x)
        {
            // Playerが左 → 左向き
            m_isReverseX = false;
        }
        else if (ninjaPos.x > m_pos.x)
        {
            // Playerが右 → 右向き
            m_isReverseX = true;
        }
    }
    else
    {
        // 待機
        m_animation.SetAnimation(
            NormalEnemyAnim::WAIT);

        // 待機時は右向き
        m_isReverseX = false;
    }

    // アニメーション更新
    m_animation.Update();

    // 射撃アニメーション中
    if (m_animation.GetCurrentAnimation() ==
        NormalEnemyAnim::SHOOT)
    {
        // 現在のアニメーションフレームを渡す
        m_attack.Update(
            m_pos,
            ninjaPos,
            m_animation.GetFrame());
    }
    else
    {
        // 射撃していない場合は攻撃状態をリセット
        m_attack.Reset();
    }
}

// 描画
void NormalEnemy::Draw()
{
    // 死亡していたら描画しない
    if (m_isDead)
    {
        return;
    }

    // 敵本体
    m_animation.DrawAnimation(
        m_pos,
        m_size,
        m_isReverseX);

    // 攻撃エフェクト
    m_attack.Draw(
        m_pos);
}

bool NormalEnemy::IsAttackActive() const
{
    return m_attack.IsAttacking();
}

VECTOR NormalEnemy::GetAttackStartPos() const
{
    VECTOR start =
        m_pos;

    start.y += 3.0f;

    return start;
}

VECTOR NormalEnemy::GetAttackTargetPos() const
{
    VECTOR start =
        GetAttackStartPos();

    VECTOR attackTarget =
        m_attack.GetAttackTargetPos();

    float dx =
        attackTarget.x -
        m_pos.x;

    float dz =
        attackTarget.z -
        m_pos.z;

    float length =
        sqrtf(
            dx * dx +
            dz * dz);

    if (length <= 0.001f)
    {
        return start;
    }

    dx /= length;
    dz /= length;

    start.x +=
        dx * 50.0f;

    start.z +=
        dz * 50.0f;

    return start;
}