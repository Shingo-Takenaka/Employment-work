#pragma once

class Animation
{
public:

    Animation();

    // 全フレーム同じ速度
    void Init(int frameMax, int interval);

    // フレームごとに速度を設定
    void Init(int frameMax, const int* intervals);

    void Update();

    int GetFrame() const;

    void Reset();

private:

    static const int MAX_FRAME = 16;

    int m_frame;

    int m_timer;

    int m_frameMax;

    // フレームごとの切り替え速度
    int m_intervals[MAX_FRAME];
};