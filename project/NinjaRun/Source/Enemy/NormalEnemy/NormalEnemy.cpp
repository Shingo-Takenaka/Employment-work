#include "NormalEnemy.h"

#include "../../Ninja/Ninja.h"

#include <math.h>

NormalEnemy::NormalEnemy()
{
    m_pos = VGet(50.0f, 0.0f, 80.0f);

    m_size = 10.0f;

    // 待機時は右向き
    m_isReverseX = false;

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
    // 待機
    LoadAnimation(
        m_animation[(int)NormalEnemyAnim::WAIT],
        "Data/Enemy/NormalEnemy/NormalEnemyWait.png",
        6,
        32,
        32,
        15);

    // 射撃
    LoadAnimation(
        m_animation[(int)NormalEnemyAnim::SHOOT],
        "Data/Enemy/NormalEnemy/NormalEnemyShoot.png",
        5,
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
void NormalEnemy::Update(const Ninja& ninja)
{
    // Playerの座標
    VECTOR ninjaPos =
        ninja.GetPosition();

    // X・Z方向の距離
    float dx =
        ninjaPos.x - m_pos.x;

    float dz =
        ninjaPos.z - m_pos.z;

    float distance =
        sqrtf(
            dx * dx +
            dz * dz);

    // 射撃判定
    const float shootRange = 50.0f;

    if (distance <= shootRange)
    {
        // 射撃
        if (m_currentAnim != NormalEnemyAnim::SHOOT)
        {
            m_currentAnim = NormalEnemyAnim::SHOOT;

            m_animation[(int)NormalEnemyAnim::SHOOT]
                .anim.Reset();
        }

        if (ninjaPos.x < m_pos.x)
        {
            // Playerが左 → 左向き
            m_isReverseX = false;
        }
        else if (ninjaPos.x > m_pos.x)
        {
            // Playerが右 → 右向き
            m_isReverseX = true;
        }
    }
    else
    {
        // 待機
        if (m_currentAnim != NormalEnemyAnim::WAIT)
        {
            m_currentAnim = NormalEnemyAnim::WAIT;

            m_animation[(int)NormalEnemyAnim::WAIT]
                .anim.Reset();
        }

        // 待機時は右向き
        m_isReverseX = false;
    }

    // 現在のアニメーション更新
    m_animation[(int)m_currentAnim]
        .anim.Update();
}

// 描画
void NormalEnemy::Draw()
{
    EnemySpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    int graph =
        anim.graph[
            anim.anim.GetFrame()];

    // 通常向き
    if (!m_isReverseX)
    {
        DrawBillboard3D(
            m_pos,
            0.5f,
            1.0f,
            m_size,
            0.0f,
            graph,
            TRUE);
    }
    // 左右反転
    else
    {
        const float halfSize =
            m_size * 0.5f;

        // m_posをそのまま基準にする
        // X方向の補正は不要
        DrawModiBillboard3D(
            m_pos,

            // 左上
            halfSize, 0.0f,

            // 右上
            -halfSize, 0.0f,

            // 右下
            -halfSize, -m_size,

            // 左下
            halfSize, -m_size,

            graph,
            TRUE);
    }
}

// 座標取得
VECTOR NormalEnemy::GetPosition() const
{
    return m_pos;
}

// 座標設定
void NormalEnemy::SetPosition(VECTOR pos)
{
    m_pos = pos;
}