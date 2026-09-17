#pragma once

#include "DxLib.h"

#include "../EnemyBase/EnemyBase.h"
#include "NormalEnemyAnimation.h"
#include "NormalEnemyAttack.h"

class Ninja;

class NormalEnemy : public EnemyBase
{
public:

    NormalEnemy();
    ~NormalEnemy();

    void Update(const Ninja& ninja) override;
    void Draw() override;

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