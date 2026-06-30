#pragma once

#include "DxLib.h"

class Field
{
public:

    Field();

    ~Field();

    void Draw();

private:

    int m_model;

    VECTOR m_pos;
    VECTOR m_rot;
    VECTOR m_scale;
};