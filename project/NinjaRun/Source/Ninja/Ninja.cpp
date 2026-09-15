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
    m_isShoot = false;

    // ガード
    m_isGuard = false;

    // ノックバック
    m_isKnockback = false;

    m_knockbackDirection =
        VGet(0.0f, 0.0f, 0.0f);

    m_knockbackStrength = 0.0f;

    m_knockbackTimer = 0.0f;

    // 手裏剣発射タイミング
    m_isShootStart = false;

    // 最初は右方向
    m_lastShootDirection = 0;
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
    // 手裏剣を投げたタイミングを毎フレームリセット
    m_isShootStart = false;

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
    if (m_input.guard &&
        !m_isJump &&
        !m_isSlash &&
        !m_isShoot)
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
        !m_isGuard &&
        !m_isShoot)
    {
        m_isJump = true;

        m_groundY = m_pos.y;

        m_jumpSpeed = 1.0f;
    }

    // 近接攻撃
    if (m_input.slash &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        m_isSlash = true;

        m_currentAnim = NinjaAnim::SLASH;

        m_animation[(int)NinjaAnim::SLASH].anim.Reset();
    }

    // 遠距離攻撃
    if (m_input.shoot &&
        !m_isShoot &&
        !m_isGuard &&
        !m_isSlash)
    {
        // 手裏剣攻撃開始
        m_isShoot = true;

        // SHOOTアニメーションを必ず最初から再生
        m_currentAnim = NinjaAnim::SHOOT;

        m_animation[(int)NinjaAnim::SHOOT].anim.Reset();

        // このフレームで手裏剣を1個発射
        m_isShootStart = true;
    }

    // 移動
    if (!m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        if (m_input.isMove)
        {
            m_pos.x += m_input.moveX * m_moveSpeed;
            m_pos.z += m_input.moveZ * m_moveSpeed;

            // 最後に移動した方向を記憶
            if (m_input.moveZ > 0.0f)
            {
                // W
                m_lastShootDirection = 2;
            }
            else if (m_input.moveZ < 0.0f)
            {
                // S
                m_lastShootDirection = 3;
            }
            else if (m_input.moveX > 0.0f)
            {
                // D
                m_lastShootDirection = 0;
            }
            else if (m_input.moveX < 0.0f)
            {
                // A
                m_lastShootDirection = 1;
            }
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
        !m_isGuard &&
        !m_isShoot)
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

bool Ninja::IsSlash() const
{
    return m_isSlash;
}

bool Ninja::IsReverseX() const
{
    return m_isReverseX;
}

bool Ninja::IsShoot() const
{
    return m_isShoot;
}

bool Ninja::IsShootStart() const
{
    return m_isShootStart;
}

int Ninja::GetShootDirection() const
{
    return m_lastShootDirection;
}