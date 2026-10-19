#include "NinjaAnimation.h"

NinjaAnimation::NinjaAnimation()
{
    m_currentAnim = NinjaAnim::WAIT;

    for (int i = 0; i < (int)NinjaAnim::MAX; i++)
    {
        m_animation[i].frameNum = 0;

        for (int j = 0; j < SpriteAnimation::MAX_FRAME; j++)
        {
            m_animation[i].graph[j] = -1;
        }
    }
}

bool NinjaAnimation::LoadAnimations()
{
    bool result = true;

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::WAIT],
        "Data/Ninja/Move/Wait/Wait.png",
        6, 32, 32, 15);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::WALK],
        "Data/Ninja/Move/Walk/Walk.png",
        9, 32, 32, 5);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::JUMP],
        "Data/Ninja/Move/Jump/Jump.png",
        6, 32, 32, 8);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::FALLING],
        "Data/Ninja/Move/Jump/falling.png",
        3, 32, 32, 8);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::SLASH],
        "Data/Ninja/Attack/Slash/Slash.png",
        7, 32, 32, 6);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::GUARD],
        "Data/Ninja/Move/Guard/Guard.png",
        2, 32, 32, 25);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::SHOOT],
        "Data/Ninja/Attack/Shoot/NinjaShoot.png",
        6, 32, 32, 6);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::DASH],
        "Data/Ninja/Move/Dash/NinjaDash.png",
        3, 32, 32, 6);

    result &= LoadAnimation(
        m_animation[(int)NinjaAnim::DASH_SLASH],
        "Data/Ninja/Attack/DashSlash/DashSlash.png",
        6, 40, 32, 3);

    return result;
}

bool NinjaAnimation::LoadAnimation(
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

    animation.anim.Init(frameNum, interval);

    return true;
}

void NinjaAnimation::Play(NinjaAnim animation)
{
    m_currentAnim = animation;

    m_animation[(int)m_currentAnim].anim.Reset();
}

void NinjaAnimation::ChangeAnimation(NinjaAnim animation)
{
    if (m_currentAnim != animation)
    {
        m_currentAnim = animation;
        m_animation[(int)m_currentAnim].anim.Reset();
    }
}

void NinjaAnimation::Update(
    bool isMove,
    bool isJump,
    float jumpSpeed,
    bool& isSlash,
    bool isGuard,
    bool& isShoot,
    bool isDash,
    bool isDashCut)
{
    if (isSlash)
    {
        SpriteAnimation& slashAnim =
            m_animation[(int)NinjaAnim::SLASH];

        if (slashAnim.anim.GetFrame() ==
            slashAnim.frameNum - 1)
        {
            isSlash = false;
        }
    }

    if (isShoot)
    {
        SpriteAnimation& shootAnim =
            m_animation[(int)NinjaAnim::SHOOT];

        if (shootAnim.anim.GetFrame() ==
            shootAnim.frameNum - 1)
        {
            isShoot = false;
        }
    }

    if (isDashCut)
    {
        ChangeAnimation(NinjaAnim::DASH_SLASH);
    }
    else if (isSlash)
    {
        ChangeAnimation(NinjaAnim::SLASH);
    }
    else if (isGuard)
    {
        ChangeAnimation(NinjaAnim::GUARD);
    }
    else if (isShoot)
    {
        ChangeAnimation(NinjaAnim::SHOOT);
    }
    else if (isJump)
    {
        if (jumpSpeed > 0.0f)
        {
            ChangeAnimation(NinjaAnim::JUMP);
        }
        else
        {
            ChangeAnimation(NinjaAnim::FALLING);

            if (jumpSpeed <= 0.0f)
            {
                m_animation[
                    (int)NinjaAnim::FALLING
                ].anim.SetFrame(1);
            }
        }
    }
    else if (isDash)
    {
        ChangeAnimation(NinjaAnim::DASH);
    }
    else if (isMove)
    {
        ChangeAnimation(NinjaAnim::WALK);
    }
    else
    {
        ChangeAnimation(NinjaAnim::WAIT);
    }

    m_animation[(int)m_currentAnim].anim.Update();
}

void NinjaAnimation::Draw(
    VECTOR pos,
    float size,
    bool isReverseX)
{
    SpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    float drawSize = size;
    
    if (m_currentAnim == NinjaAnim::DASH_SLASH)
    {
        drawSize = size * 1.25f;
    }

    if (m_currentAnim == NinjaAnim::SLASH)
    {
        switch (anim.anim.GetFrame())
        {
        case 1:
        case 2:
        case 3:
            drawSize = 11.0f;
            break;

        case 4:
            drawSize = 12.0f;
            break;
        }
    }

    DrawBillboard3D(
        pos,
        0.5f,
        0.0f,
        drawSize,
        0.0f,
        anim.graph[anim.anim.GetFrame()],
        TRUE,
        isReverseX);
}