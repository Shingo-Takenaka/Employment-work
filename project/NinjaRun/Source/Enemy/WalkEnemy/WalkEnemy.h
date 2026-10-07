#pragma once

#include "DxLib.h"

#include "../EnemyBase/EnemyBase.h"
#include "WalkEnemyAnimation.h"

class Ninja;

class WalkEnemy : public EnemyBase
{
public:

    WalkEnemy();
    ~WalkEnemy();

    void Update(const Ninja& ninja) override;
    void Draw() override;

    void SetPosition(VECTOR pos);

private:

    float m_moveSpeed;

    float m_minX;
    float m_maxX;

    bool m_isMoveLeft;

    WalkEnemyAnimation m_animation;
};