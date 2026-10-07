#pragma once

#include "DxLib.h"

class Camera
{
public:
    Camera();

    void Update(VECTOR playerPos);
    void Draw();

private:
    VECTOR m_CameraPos;
    VECTOR m_target;

    VECTOR m_offset;
};