#pragma once

#include "../DrawManager/DrawManager.h"
#include "../Camera/Camera.h"
#include "../Field/Field.h"
#include "../Ninja/Ninja.h"
#include "../Ninja/NinjaAttack.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"
#include "../Collision/TouchEnemy.h"
#include "../Collision/PlayerAttackCollision.h"
#include "../Collision/ShootCollision.h"
#include "../Collision/EnemyAttackCollision.h"
#include "../Enemy/WalkEnemy/WalkEnemy.h"

class PlayScene
{
public:

    PlayScene();
    ~PlayScene();

    void Update();
    void Draw();

private:

    Camera m_camera;

    Field m_field;

    Ninja m_ninja;

    NormalEnemy m_enemy;

    WalkEnemy m_walkEnemy;

    bool m_isTouchEnemy;

    // PlayerUŒ‚”»’è
    PlayerAttackCollision m_playerAttackCollision;

    // Player‚Ìè— Œ•
    NinjaAttack m_playerAttack;

    // •`‰æŠÇ—
    DrawManager m_drawManager;
};