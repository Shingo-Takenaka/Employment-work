#pragma once

#include <vector>

class Ninja;
class EnemyBase;

class DashAttackCollision
{
public:
    DashAttackCollision();

    void Update(const Ninja& ninja);

    // ダッシュ切りと敵の当たり判定
    bool Check(const Ninja& ninja, const EnemyBase& enemy);

private:
    float m_attackWidth;
    float m_attackDepth;
    float m_attackHeight;

    // ダッシュ切り中に攻撃した敵
    std::vector<const EnemyBase*> m_hitEnemies;
};