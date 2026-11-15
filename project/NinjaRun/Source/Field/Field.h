#pragma once

#include "DxLib.h"

class Field
{
public:
    Field();
    ~Field();

    void Draw();

    int GetModelHandle() const;

private:
    void Load();
    void SetupModel();

    int m_model;

    VECTOR m_fieldPos;
    VECTOR m_fieldRot;
    VECTOR m_fieldScale;
};
