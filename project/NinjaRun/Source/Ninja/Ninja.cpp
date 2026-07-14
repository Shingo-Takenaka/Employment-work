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
}

Ninja::~Ninja()
{
    for (int anim = 0; anim < (int)NinjaAnim::MAX; anim++)
    {
        for (int frame = 0; frame < m_animation[anim].frameNum; frame++)
        {
            DeleteGraph(
                m_animation[anim].graph[frame]);
        }
    }
}

void Ninja::Update()
{
    bool isMove = false;
    bool jump = false;
    bool slash = false;

    float moveX = 0.0f;
    float moveZ = 0.0f;

    // 入力
    UpdateInput(
        isMove,
        jump,
        slash,
        moveX,
        moveZ);

    // ジャンプ開始
    if (jump && !m_isJump && !m_isSlash)
    {
        m_isJump = true;

        m_groundY = m_pos.y;

        m_jumpSpeed = 1.0f;
    }

    // 攻撃開始
    if (slash && !m_isSlash)
    {
        m_isSlash = true;

        m_currentAnim = NinjaAnim::SLASH;

        m_animation[(int)NinjaAnim::SLASH].anim.Reset();
    }

    // 移動
    if (!m_isSlash)
    {
        if (moveX != 0.0f || moveZ != 0.0f)
        {
            m_pos.x += moveX * m_moveSpeed;
            m_pos.z += moveZ * m_moveSpeed;

            isMove = true;
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
    if (!m_isSlash)
    {
        if (moveX < 0.0f)
        {
            m_isReverseX = true;
        }
        else if (moveX > 0.0f)
        {
            m_isReverseX = false;
        }
    }

    // アニメーション更新
    UpdateAnimation(isMove);
}

void Ninja::Draw()
{
    DrawAnimation();
}

VECTOR Ninja::GetPosition() const
{
    return m_pos;
}