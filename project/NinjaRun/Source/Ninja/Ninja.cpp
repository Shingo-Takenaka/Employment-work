#include "Ninja.h"

Ninja::Ninja()
{
    // 基本設定
    m_pos = VGet(0.0f, 0.0f, 0.0f);

    m_moveSpeed = 0.5f;

    m_size = 10.0f;

    // アニメーション読み込み
    LoadAnimations();

    m_currentAnim = NinjaAnim::WAIT;

    // 向き
    m_isReverseX = false;

    // ジャンプ
    m_isJump = false;

    m_groundY = m_pos.y;

    m_jumpSpeed = 0.0f;

    m_gravity = 0.05f;

    // 攻撃
    m_isSlash = false;

    // ガード
    m_isGuard = false;

    // ノックバック
    m_isKnockback = false;

    m_knockbackDirection =
        VGet(0.0f, 0.0f, 0.0f);

    m_knockbackStrength = 0.0f;

    m_knockbackTimer = 0.0f;
}

Ninja::~Ninja()
{
    for (int anim = 0; anim < (int)NinjaAnim::MAX; anim++)
    {
        for (int frame = 0; frame < m_animation[anim].frameNum; frame++)
        {
            DeleteGraph(m_animation[anim].graph[frame]);
        }
    }
}

void Ninja::Update()
{
    // ノックバック中なら通常操作を行わない
    if (m_isKnockback)
    {
        m_pos.x +=
            m_knockbackDirection.x *
            m_knockbackStrength;

        m_pos.z +=
            m_knockbackDirection.z *
            m_knockbackStrength;

        m_knockbackTimer -= 1.0f / 60.0f;

        if (m_knockbackTimer <= 0.0f)
        {
            m_knockbackTimer = 0.0f;
            m_isKnockback = false;
            m_knockbackStrength = 0.0f;
        }

        UpdateAnimation(false);

        return;
    }

    // 入力更新
    UpdateInput();

    // ガード
    if (m_input.guard && !m_isJump && !m_isSlash)
    {
        m_isGuard = true;
    }
    else
    {
        m_isGuard = false;
    }

    // ジャンプ
    if (m_input.jump &&
        !m_isJump &&
        !m_isSlash &&
        !m_isGuard)
    {
        m_isJump = true;

        m_groundY = m_pos.y;

        m_jumpSpeed = 1.0f;
    }

    // 攻撃
    if (m_input.slash &&
        !m_isSlash &&
        !m_isGuard)
    {
        m_isSlash = true;

        m_currentAnim = NinjaAnim::SLASH;

        m_animation[(int)NinjaAnim::SLASH].anim.Reset();
    }

    // 移動
    if (!m_isSlash &&
        !m_isGuard)
    {
        if (m_input.isMove)
        {
            m_pos.x += m_input.moveX * m_moveSpeed;
            m_pos.z += m_input.moveZ * m_moveSpeed;
        }
    }

    // ジャンプ処理
    if (m_isJump)
    {
        m_pos.y += m_jumpSpeed;

        m_jumpSpeed -= m_gravity;

        if (m_pos.y <= m_groundY)
        {
            m_pos.y = m_groundY;

            m_isJump = false;

            m_jumpSpeed = 0.0f;
        }
    }

    // 左右反転
    if (!m_isSlash &&
        !m_isGuard)
    {
        if (m_input.moveX < 0.0f)
        {
            m_isReverseX = true;
        }
        else if (m_input.moveX > 0.0f)
        {
            m_isReverseX = false;
        }
    }

    // アニメーション更新
    UpdateAnimation(m_input.isMove);
}

void Ninja::Draw()
{
    DrawAnimation();
}

VECTOR Ninja::GetPosition() const
{
    return m_pos;
}

void Ninja::ApplyKnockback(
    VECTOR direction,
    float strength,
    float duration)
{
    m_isKnockback = true;

    m_knockbackDirection = direction;

    m_knockbackStrength = strength;

    m_knockbackTimer = duration;
}

bool Ninja::IsKnockback() const
{
    return m_isKnockback;
}