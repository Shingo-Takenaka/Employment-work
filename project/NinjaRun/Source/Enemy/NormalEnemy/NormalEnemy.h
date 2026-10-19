#pragma once

#include "DxLib.h"
#include "../EnemyBase/EnemyBase.h"
#include "NormalEnemyAnimation.h"
#include "NormalEnemyAttack.h"
#include "../../Collision/NinjaAttack/AttackCollision.h"
#include "../../Collision/NinjaAttack/DashAttackCollision.h"
#include "../../Ninja/Ninja.h"

#include <math.h>

class Ninja;
class PlayerAttackCollision;
class DashAttackCollision;

class NormalEnemy : public EnemyBase
{
public:

    NormalEnemy();
    ~NormalEnemy();

    void Update(const Ninja& ninja) override;
    void Draw() override;

    // Playerの通常攻撃・ダッシュ切り判定
    void CheckPlayerAttack(
        const Ninja& ninja,
        PlayerAttackCollision& playerAttackCollision,
        DashAttackCollision& dashAttackCollision);

    // Enemy攻撃の当たり判定が有効か
    bool IsAttackActive() const;

    // 攻撃開始位置を取得
    VECTOR GetAttackStartPos() const;

    // 攻撃終了位置を取得
    VECTOR GetAttackTargetPos() const;

private:

    // Player座標
    VECTOR m_playerPos;

    // アニメーション
    NormalEnemyAnimation m_animation;

    // 攻撃
    NormalEnemyAttack m_attack;
};