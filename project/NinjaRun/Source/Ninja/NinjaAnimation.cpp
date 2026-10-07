#include "Ninja.h"

void Ninja::LoadAnimations()
{
    // 待機
    LoadAnimation(
        m_animation[(int)NinjaAnim::WAIT],
        "Data/Ninja/Wait/Wait.png",
        6,
        32,
        32,
        15);

    // 歩き
    LoadAnimation(
        m_animation[(int)NinjaAnim::WALK],
        "Data/Ninja/Walk/Walk.png",
        9,
        32,
        32,
        5);

    // ジャンプ上昇
    LoadAnimation(
        m_animation[(int)NinjaAnim::JUMP],
        "Data/Ninja/Jump/Jump.png",
        6,
        32,
        32,
        8);

    // ジャンプ落下
    LoadAnimation(
        m_animation[(int)NinjaAnim::FALLING],
        "Data/Ninja/Jump/falling.png",
        3,
        32,
        32,
        8);

    // 袈裟斬り
    LoadAnimation(
        m_animation[(int)NinjaAnim::SLASH],
        "Data/Ninja/Slash/Slash.png",
        7,
        32,
        32,
        6);

    // ガード
    LoadAnimation(
        m_animation[(int)NinjaAnim::GUARD],
        "Data/Ninja/Guard/Guard.png",
        2,
        32,
        32,
        25);

    // 手裏剣
    LoadAnimation(
        m_animation[(int)NinjaAnim::SHOOT],
        "Data/Ninja/Shoot/NinjaShoot.png",
        6,
        32,
        32,
        6);
}

bool Ninja::LoadAnimation(
    SpriteAnimation& animation,
    const char* fileName,
    int frameNum,
    int width,
    int height,
    int interval)
{
    animation.frameNum = frameNum;

    if (LoadDivGraph(
        fileName,
        frameNum,
        frameNum,
        1,
        width,
        height,
        animation.graph) == -1)
    {
        return false;
    }

    animation.anim.Init(
        frameNum,
        interval);

    return true;
}

void Ninja::UpdateAnimation(bool isMove)
{
    // 近接攻撃
    if (m_isSlash)
    {
        SpriteAnimation& slashAnim =
            m_animation[(int)NinjaAnim::SLASH];

        // アニメーション最後まで再生したら攻撃終了
        if (slashAnim.anim.GetFrame() ==
            slashAnim.frameNum - 1)
        {
            m_isSlash = false;
        }
    }

    // 手裏剣
    if (m_isShoot)
    {
        SpriteAnimation& shootAnim =
            m_animation[(int)NinjaAnim::SHOOT];

        // アニメーション最後まで再生したら攻撃終了
        if (shootAnim.anim.GetFrame() ==
            shootAnim.frameNum - 1)
        {
            m_isShoot = false;
        }
    }

    // アニメーション切り替え
    if (m_isSlash)
    {
        if (m_currentAnim != NinjaAnim::SLASH)
        {
            m_currentAnim = NinjaAnim::SLASH;

            m_animation[
                (int)m_currentAnim
            ].anim.Reset();
        }
    }
    else if (m_isGuard)
    {
        if (m_currentAnim != NinjaAnim::GUARD)
        {
            m_currentAnim = NinjaAnim::GUARD;

            m_animation[
                (int)m_currentAnim
            ].anim.Reset();
        }
    }
    else if (m_isShoot)
    {
        if (m_currentAnim != NinjaAnim::SHOOT)
        {
            m_currentAnim = NinjaAnim::SHOOT;

            m_animation[
                (int)m_currentAnim
            ].anim.Reset();
        }
    }
    else if (m_isJump)
    {
        if (m_jumpSpeed > 0.0f)
        {
            if (m_currentAnim != NinjaAnim::JUMP)
            {
                m_currentAnim = NinjaAnim::JUMP;

                m_animation[
                    (int)m_currentAnim
                ].anim.Reset();
            }
        }
        else
        {
            if (m_currentAnim != NinjaAnim::FALLING)
            {
                m_currentAnim = NinjaAnim::FALLING;

                m_animation[
                    (int)m_currentAnim
                ].anim.Reset();
            }

            // 落下速度が0以下になるまでは2枚目で止める
            if (m_jumpSpeed <= 0.0f)
            {
                m_animation[
                    (int)NinjaAnim::FALLING
                ].anim.SetFrame(1);
            }
        }
    }
    else if (isMove)
    {
        if (m_currentAnim != NinjaAnim::WALK)
        {
            m_currentAnim = NinjaAnim::WALK;

            m_animation[
                (int)m_currentAnim
            ].anim.Reset();
        }
    }
    else
    {
        if (m_currentAnim != NinjaAnim::WAIT)
        {
            m_currentAnim = NinjaAnim::WAIT;

            m_animation[
                (int)m_currentAnim
            ].anim.Reset();
        }
    }

    // 現在のアニメーション更新
    m_animation[
        (int)m_currentAnim
    ].anim.Update();
}

void Ninja::DrawAnimation()
{
    SpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    // 通常サイズ
    float drawSize = m_size;

    // Slash中だけサイズを変更
    if (m_currentAnim == NinjaAnim::SLASH)
    {
        switch (anim.anim.GetFrame())
        {
        case 1:
            drawSize = 11.0f;
            break;

        case 2:
            drawSize = 11.0f;
            break;

        case 3:
            drawSize = 11.0f;
            break;

        case 4:
            drawSize = 12.0f;
            break;
        }
    }

    DrawBillboard3D(
        m_pos,
        0.5f,
        0.0f,
        drawSize,
        0.0f,
        anim.graph[anim.anim.GetFrame()],
        TRUE,
        m_isReverseX);
}