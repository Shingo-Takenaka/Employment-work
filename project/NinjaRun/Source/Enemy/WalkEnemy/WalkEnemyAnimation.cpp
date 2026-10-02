
#include "WalkEnemyAnimation.h"

// コンストラクタ
WalkEnemyAnimation::WalkEnemyAnimation()
{
    m_currentAnim = WalkEnemyAnim::WAIT;

    for (int anim = 0;
        anim < (int)WalkEnemyAnim::MAX;
        anim++)
    {
        m_animation[anim].frameNum = 0;

        for (int frame = 0;
            frame < WalkEnemySpriteAnimation::MAX_FRAME;
            frame++)
        {
            m_animation[anim].graph[frame] = -1;
        }
    }
}

// デストラクタ
WalkEnemyAnimation::~WalkEnemyAnimation()
{
    for (int anim = 0;
        anim < (int)WalkEnemyAnim::MAX;
        anim++)
    {
        for (int frame = 0;
            frame < m_animation[anim].frameNum;
            frame++)
        {
            if (m_animation[anim].graph[frame] != -1)
            {
                DeleteGraph(
                    m_animation[anim].graph[frame]);
            }
        }
    }
}

// アニメーション読み込み
void WalkEnemyAnimation::LoadAnimations()
{
    // 待機
    LoadAnimation(
        m_animation[(int)WalkEnemyAnim::WAIT],
        "Data/Enemy/WalkEnemy/WalkEnemyWait.png",
        6,
        32,
        32,
        15);

    // 歩行
    LoadAnimation(
        m_animation[(int)WalkEnemyAnim::WALK],
        "Data/Enemy/WalkEnemy/WalkEnemyWalk.png",
        6,
        32,
        32,
        13);
}

// アニメーション1種類読み込み
bool WalkEnemyAnimation::LoadAnimation(
    WalkEnemySpriteAnimation& animation,
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
        animation.frameNum = 0;
        return false;
    }

    animation.anim.Init(
        frameNum,
        interval);

    return true;
}

// アニメーション更新
void WalkEnemyAnimation::Update()
{
    m_animation[(int)m_currentAnim]
        .anim.Update();
}

// アニメーション変更
void WalkEnemyAnimation::SetAnimation(
    WalkEnemyAnim anim)
{
    if (m_currentAnim == anim)
    {
        return;
    }

    m_currentAnim = anim;

    m_animation[(int)m_currentAnim]
        .anim.Reset();
}

// 現在のアニメーション取得
WalkEnemyAnim
WalkEnemyAnimation::GetCurrentAnimation() const
{
    return m_currentAnim;
}

// 現在のフレーム取得
int WalkEnemyAnimation::GetFrame() const
{
    return m_animation[(int)m_currentAnim]
        .anim.GetFrame();
}

// リセット
void WalkEnemyAnimation::Reset()
{
    m_animation[(int)m_currentAnim]
        .anim.Reset();
}

// 描画
void WalkEnemyAnimation::DrawAnimation(
    VECTOR pos,
    float size,
    bool isReverseX)
{
    WalkEnemySpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    int frame = anim.anim.GetFrame();

    if (anim.frameNum <= 0 ||
        frame < 0 ||
        frame >= anim.frameNum)
    {
        return;
    }

    int graph = anim.graph[frame];

    const float halfSize = size * 0.5f;

    // 通常向き
    if (!isReverseX)
    {
        DrawModiBillboard3D(
            pos,

            // 右上
            -halfSize, size,

            // 左上
            halfSize, size,

            // 左下
            halfSize, 0.0f,

            // 右下
            -halfSize, 0.0f,

            graph,
            TRUE);
    }
    // 左右反転
    else
    {
        DrawModiBillboard3D(
            pos,

            // 右上
            halfSize, size,

            // 左上
            -halfSize, size,

            // 左下
            -halfSize, 0.0f,

            // 右下
            halfSize, 0.0f,

            graph,
            TRUE);
    }
}