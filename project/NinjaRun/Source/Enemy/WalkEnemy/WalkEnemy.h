#pragma once

#include "DxLib.h"

#include "../EnemyBase/EnemyBase.h"
#include "WalkEnemyAnimation.h"
#include "../../Collision/NinjaAttack/AttackCollision.h"
#include "../../Collision/NinjaAttack/DashAttackCollision.h"

class Ninja;
class PlayerAttackCollision;
class DashAttackCollision;

class WalkEnemy : public EnemyBase
{
public:

    WalkEnemy();
    ~WalkEnemy();

    void Update(const Ninja& ninja) override;
    void Draw() override;
    // Playerの通常攻撃・ダッシュ切り判定
    void CheckPlayerAttack(const Ninja& ninja,PlayerAttackCollision& playerAttackCollision,DashAttackCollision& dashAttackCollision);

    void SetPosition(VECTOR pos);

private:

    float m_moveSpeed;

    float m_minX;
    float m_maxX;

    bool m_isMoveLeft;

    WalkEnemyAnimation m_animation;
};