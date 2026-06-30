#include "Animation.h"

Animation::Animation()
{
    m_frame = 0;
    m_timer = 0;
    m_frameMax = 1;
    m_interval = 1;
}

void Animation::Init(int frameMax, int interval)
{
    m_frame = 0;
    m_timer = 0;
    m_frameMax = frameMax;
    m_interval = interval;
}

void Animation::Update()
{
    m_timer++;

    if (m_timer >= m_interval)
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