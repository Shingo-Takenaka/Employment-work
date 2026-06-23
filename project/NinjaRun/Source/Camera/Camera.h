#pragma once

#include "DxLib.h"

class Camera
{
public:
    Camera();

    void Update();
    void Draw();

private:
    VECTOR m_eye;
    VECTOR m_target;
};