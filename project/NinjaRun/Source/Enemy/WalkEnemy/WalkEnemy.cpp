#include "WalkEnemy.h"

#include "../../Ninja/Ninja.h"

WalkEnemy::WalkEnemy()
    : EnemyBase(1)
{
    m_pos = VGet(50.0f, 0.0f, 50.0f);

    // 移動速度
    m_moveSpeed = 0.2f;

    // 移動範囲
    m_minX = m_pos.x - 25.0f;
    m_maxX = m_pos.x + 25.0f;

    // 初期状態では右方向へ移動
    m_isMoveLeft = false;
    m_isReverseX = false;

    // アニメーション読み込み
    m_animation.LoadAnimations();

}

WalkEnemy::~WalkEnemy()
{
}

// 更新
void WalkEnemy::Update(const Ninja& ninja)
{
    // 死亡していたら更新しない
    if (m_isDead)
    {
        return;
    }

    // 左右に移動
    if (m_isMoveLeft)
    {
        m_pos.x -= m_moveSpeed;

        // 左端に到達したら右向きに変更
        if (m_pos.x <= m_minX)
        {
            m_pos.x = m_minX;
            m_isMoveLeft = false;
        }
    }
    else
    {
        m_pos.x += m_moveSpeed;

        // 右端に到達したら左向きに変更
        if (m_pos.x >= m_maxX)
        {
            m_pos.x = m_maxX;
            m_isMoveLeft = true;
        }
    }

    // 移動方向に合わせて向きを変更
    m_isReverseX = m_isMoveLeft;

    // 歩くアニメーション
    m_animation.SetAnimation(WalkEnemyAnim::WALK);

    // アニメーション更新
    m_animation.Update();

}

// 描画
void WalkEnemy::Draw()
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

}