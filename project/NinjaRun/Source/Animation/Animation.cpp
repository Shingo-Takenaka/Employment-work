#include "Animation.h"

Animation::Animation()
{
    m_frame = 0;
    m_timer = 0;
    m_frameMax = 1;

    for (int i = 0; i < MAX_FRAME; i++)
    {
        m_intervals[i] = 1;
    }
}

// 全フレーム同じ速度
void Animation::Init(
    int frameMax,
    int interval)
{
    m_frame = 0;
    m_timer = 0;
    m_frameMax = frameMax;

    for (int i = 0; i < MAX_FRAME; i++)
    {
        m_intervals[i] = interval;
    }
}

// フレームごとに速度を設定
void Animation::Init(
    int frameMax,
    const int* intervals)
{
    m_frame = 0;
    m_timer = 0;
    m_frameMax = frameMax;

    for (int i = 0; i < MAX_FRAME; i++)
    {
        if (i < frameMax)
        {
            m_intervals[i] = intervals[i];
        }
        else
        {
            m_intervals[i] = 1;
        }
    }
}

void Animation::Update()
{
    m_timer++;

    // 現在のフレームの切り替え時間
    if (m_timer >= m_intervals[m_frame])
    {
        m_timer = 0;

        m_frame++;

        if (m_frame >= m_frameMax)
        {
            m_frame = 0;
        }
    }
}

int Animation::GetFrame() const
{
    return m_frame;
}

void Animation::Reset()
{
    m_frame = 0;
    m_timer = 0;
}