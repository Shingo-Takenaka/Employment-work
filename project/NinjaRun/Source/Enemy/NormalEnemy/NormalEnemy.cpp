#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
    m_pos = VGet(20.0f, 0.0f, 0.0f);

    m_size = 10.0f;

    LoadAnimations();

    m_currentAnim = NormalEnemyAnim::WAIT;
}

NormalEnemy::~NormalEnemy()
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
void NormalEnemy::LoadAnimations()
{
    LoadAnimation(
        m_animation[(int)NormalEnemyAnim::WAIT],
        "Data/Enemy/NormalEnemy/NormalEnemyWait.png",
        6,
        32,
        32,
        15);
}

// アニメーション読み込み
bool NormalEnemy::LoadAnimation(
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

// 更新
void NormalEnemy::Update()
{
    m_animation[(int)m_currentAnim]
        .anim.Update();
}

// 描画
void NormalEnemy::Draw()
{
    EnemySpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    DrawBillboard3D(
        m_pos,
        0.5f,
        1.0f,
        m_size,
        0.0f,
        anim.graph[
            anim.anim.GetFrame()],
            TRUE);
}

// 座標取得
VECTOR NormalEnemy::GetPosition() const
{
    return m_pos;
}

void NormalEnemy::SetPosition(VECTOR pos)
{
    m_pos = pos;
}