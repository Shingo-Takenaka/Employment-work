#pragma once

#include "DxLib.h"

class Field
{
public:

    Field();

    ~Field();

    void Draw();

    VECTOR GetWallPosition() const;

private:

    int m_model;
    int m_wall;

    VECTOR m_fieldPos;
    VECTOR m_fieldRot;
    VECTOR m_fieldScale;

    VECTOR m_wallPos;
    VECTOR m_wallRot;
    VECTOR m_wallScale;
};