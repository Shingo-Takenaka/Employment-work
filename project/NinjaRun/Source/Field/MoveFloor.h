
#pragma once

#include "DxLib.h"

class MoveFloor
{
public:
    MoveFloor();
    ~MoveFloor();

    void Init();
    bool Load();
    void Update();
    void Draw() const;

    VECTOR GetPosition() const { return m_position; }
    VECTOR GetMoveAmount() const { return m_moveAmount; }
    int GetModelHandle() const { return m_modelHandle; }

private:
    int m_modelHandle;

    VECTOR m_position;
    VECTOR m_startPosition;
    VECTOR m_endPosition;
    VECTOR m_moveAmount;

    float m_speed;
    bool m_isMovingToEnd;
};
