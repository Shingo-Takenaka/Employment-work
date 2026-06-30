#pragma once

class Animation
{
public:

    Animation();

    void Init(int frameMax, int interval);

    void Update();

    int GetFrame() const;

    void Reset();

private:

    int m_frame;

    int m_timer;

    int m_frameMax;

    int m_interval;
};