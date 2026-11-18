#pragma once

#include <vector>

class Ninja;
class EnemyBase;

class PlayerAttackCollision
{
public:
    PlayerAttackCollision();

    void Update(const Ninja& ninja);

    // ’ÊíaŒ‚‚ÆEnemy‚Ì“–‚½‚è”»’è
    bool Check(const Ninja& ninja, const EnemyBase& enemy);

private:
    // ’ÊíaŒ‚‚ª–½’†‚µ‚½Enemy
    std::vector<const EnemyBase*> m_hitEnemies;

    // ‘OƒtƒŒ[ƒ€‚ÌUŒ‚ó‘Ô
    bool m_wasSlashing;
};
