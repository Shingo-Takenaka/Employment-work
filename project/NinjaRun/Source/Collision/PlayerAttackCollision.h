
#pragma once

#include "../Ninja/NinjaAttack.h"

class Ninja;
class EnemyBase;

class PlayerAttackCollision
{
public:
    PlayerAttackCollision();

    // Player‚ÌUŒ‚‚ÆEnemy‚ª“–‚½‚Á‚Ä‚¢‚é‚©
    bool Check(
        const Ninja& ninja,
        const EnemyBase& enemy);

    // UŒ‚”ÍˆÍ‚ğ•\¦
    void DrawDebug(
        const Ninja& ninja);

private:
    // Player‚ÌUŒ‚”ÍˆÍ
    NinjaAttack m_playerAttack;
};