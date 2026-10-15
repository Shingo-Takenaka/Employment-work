#pragma once

#include "DxLib.h"

class Ninja;

class EnemyBase
{
public:
    EnemyBase(int hp);
    virtual ~EnemyBase();

    virtual void Update(const Ninja& ninja) = 0;
    virtual void Draw() = 0;

    VECTOR GetPosition() const;
    void SetPosition(VECTOR pos);

    void TakeDamage(int damage);
    bool IsDead() const;

protected:
    VECTOR m_pos;
    float m_size;
    bool m_isReverseX;
    int m_hp;
    bool m_isDead;
};