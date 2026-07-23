#include "Ninja.h"

void Ninja::LoadAnimations()
{
    // 待機
    LoadAnimation(
        m_animation[(int)NinjaAnim::WAIT],
        "Data/Ninja/Wait/Wait.png",
		6,      //総フレーム数
        32,     //画像サイズ
        32,     //画像サイズ
        15);    //切り替えスピード

    // 歩き
    LoadAnimation(
        m_animation[(int)NinjaAnim::WALK],
        "Data/Ninja/Walk/Walk.png",
        9,
        32,
        32,
        5);

    // ジャンプ
    LoadAnimation(
        m_animation[(int)NinjaAnim::JUMP],
        "Data/Ninja/Jump/Jump.png",
        9,
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
}

// 1つのアニメーションを読み込む
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

// アニメーション更新
void Ninja::UpdateAnimation(bool isMove)
{
    // 攻撃
    if (m_isSlash)
    {
        SpriteAnimation& slashAnim =
            m_animation[(int)NinjaAnim::SLASH];

        // 最後まで再生したら攻撃終了
        if (slashAnim.anim.GetFrame() ==
            slashAnim.frameNum - 1)
        {
            m_isSlash = false;
        }
    }

    // アニメーション切り替え
    if (m_isSlash)
    {
        if (m_currentAnim != NinjaAnim::SLASH)
        {
            m_currentAnim = NinjaAnim::SLASH;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }
    else if (m_isGuard)
    {
        if (m_currentAnim != NinjaAnim::GUARD)
        {
            m_currentAnim = NinjaAnim::GUARD;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }
    else if (m_isJump)
    {
        if (m_currentAnim != NinjaAnim::JUMP)
        {
            m_currentAnim = NinjaAnim::JUMP;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }
    else if (isMove)
    {
        if (m_currentAnim != NinjaAnim::WALK)
        {
            m_currentAnim = NinjaAnim::WALK;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }
    else
    {
        if (m_currentAnim != NinjaAnim::WAIT)
        {
            m_currentAnim = NinjaAnim::WAIT;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }

    // 現在のアニメーション更新
    m_animation[(int)m_currentAnim].anim.Update();
}

// 描画
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
		case 1:     // 2枚目
            drawSize = 11.0f;
			break;

        case 2:     // 3枚目
            drawSize = 11.0f;
            break;

        case 3:     // 4枚目
            drawSize = 11.0f;
            break;

        case 4:     // 5枚目
            drawSize = 12.0f;
            break;
        }
    }

    DrawBillboard3D(
        m_pos,                                  // 描画位置
        0.5f,                                   // 横方向基準
        1.0f,                                   // 縦方向基準
        drawSize,                               // 描画サイズ
        0.0f,                                   // 回転
        anim.graph[anim.anim.GetFrame()],       // 現在の画像
        TRUE,                                   // 透過
        m_isReverseX);                          // 左右反転
}