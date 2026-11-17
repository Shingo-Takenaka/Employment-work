#pragma once

#include "DxLib.h"

class BreakableObject
{
public:
    BreakableObject();
    ~BreakableObject();

    void Init();
    bool Load();
    void Draw() const;

    void TakeDamage(int damage);

    bool IsBroken() const { return m_isBroken; }
    int GetHP() const { return m_hp; }
    VECTOR GetPosition() const { return m_position; }
    int GetModelHandle() const { return m_modelHandle; }

private:
    int m_modelHandle;

    VECTOR m_position;

    int m_hp;
    int m_maxHP;

    bool m_isBroken;
};
