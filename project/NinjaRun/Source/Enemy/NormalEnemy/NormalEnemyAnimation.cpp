#include "NormalEnemyAnimation.h"

// コンストラクタ
NormalEnemyAnimation::NormalEnemyAnimation()
{
    m_currentAnim = NormalEnemyAnim::WAIT;
}

// デストラクタ
NormalEnemyAnimation::~NormalEnemyAnimation()
{
    for (int anim = 0;
        anim < (int)NormalEnemyAnim::MAX;
        anim++)
    {
        for (int frame = 0;
            frame < m_animation[anim].frameNum;
            frame++)
        {
            DeleteGraph(
                m_animation[anim].graph[frame]);
        }
    }
}

// アニメーション読み込み
void NormalEnemyAnimation::LoadAnimations()
{
    // 待機
    LoadAnimation(
        m_animation[(int)NormalEnemyAnim::WAIT],
        "Data/Enemy/NormalEnemy/NormalEnemyWait.png",
        6,
        32,
        32,
        15);

    // 射撃
    EnemySpriteAnimation& shoot =
        m_animation[(int)NormalEnemyAnim::SHOOT];

    shoot.frameNum = 5;

    if (LoadDivGraph(
        "Data/Enemy/NormalEnemy/NormalEnemyShoot.png",
        5,//総枚数
        5,//横に5枚
        1,//縦に1枚
        32,
        32,
        shoot.graph) == -1)
    {
        return;
    }

    // 射撃アニメーションの各フレーム速度
    const int shootInterval[5] =
    {
        15, // 1枚目
        100, // 2枚目
        15, // 3枚目
        15, // 4枚目
        15  // 5枚目
    };

    shoot.anim.Init(
        5,
        shootInterval);
}

// アニメーション1種類読み込み
bool NormalEnemyAnimation::LoadAnimation(
    EnemySpriteAnimation& animation,
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
void NormalEnemyAnimation::Update()
{
    m_animation[(int)m_currentAnim]
        .anim.Update();
}

// アニメーション変更
void NormalEnemyAnimation::SetAnimation(
    NormalEnemyAnim anim)
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
NormalEnemyAnim
NormalEnemyAnimation::GetCurrentAnimation() const
{
    return m_currentAnim;
}

// 現在のフレーム取得
int NormalEnemyAnimation::GetFrame() const
{
    return m_animation[(int)m_currentAnim]
        .anim.GetFrame();
}

// リセット
void NormalEnemyAnimation::Reset()
{
    m_animation[(int)m_currentAnim]
        .anim.Reset();
}

// 描画
// 描画
void NormalEnemyAnimation::DrawAnimation(
    VECTOR pos,
    float size,
    bool isReverseX)
{
    EnemySpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    int frame =
        anim.anim.GetFrame();

    int graph =
        anim.graph[frame];

    // 通常向き
    if (!isReverseX)
    {
        DrawBillboard3D(
            pos,
            0.5f,
            1.0f,
            size,
            0.0f,
            graph,
            TRUE);
    }
    // 左右反転
    else
    {
        const float halfSize =
            size * 0.5f;

        DrawModiBillboard3D(
            pos,

            // 左上
            halfSize, 0.0f,

            // 右上
            -halfSize, 0.0f,

            // 右下
            -halfSize, -size,

            // 左下
            halfSize, -size,

            graph,
            TRUE);
    }
}