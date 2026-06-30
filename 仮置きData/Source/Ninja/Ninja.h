#pragma once

#include "DxLib.h"
#include "../Animation/Animation.h"

class Ninja
{
public:
    Ninja();
    ~Ninja();

    void Update();
    void Draw();

    VECTOR GetPosition() const;

private:

    VECTOR m_pos;

    float m_size;
    float m_moveSpeed;

    static const int WAIT_FRAME = 6;

    int m_waitGraph[WAIT_FRAME];

    Animation m_waitAnim;
};