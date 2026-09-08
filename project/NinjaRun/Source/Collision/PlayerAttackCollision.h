#pragma once

#include "../Ninja/PlayerAttack.h"

class Ninja;
class NormalEnemy;

class PlayerAttackCollision
{
public:

    PlayerAttackCollision();

    // Player‚ÌUŒ‚‚ÆEnemy‚ª“–‚½‚Á‚Ä‚¢‚é‚©
    bool Check(
        const Ninja& ninja,
        const NormalEnemy& enemy);

    // UŒ‚”ÍˆÍ‚ğ•\¦
    void DrawDebug(
        const Ninja& ninja);

private:

    // Player‚ÌUŒ‚”ÍˆÍ
    PlayerAttack m_playerAttack;
};