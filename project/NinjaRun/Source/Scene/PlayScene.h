#pragma once

#include "../DrawManager/DrawManager.h"
#include "../Camera/Camera.h"
#include "../Field/Field.h"
#include "../Ninja/Ninja.h"
#include "../Ninja/NinjaAttack.h"
#include "../Ninja/NinjaAnimation.h"
#include "../Enemy/EnemyManager/EnemyManager.h"
#include "../Collision/TouchEnemy/TouchEnemy.h"
#include "../Collision/NinjaAttack/AttackCollision.h"
#include "../Collision/NinjaAttack/ShootCollision.h"
#include "../Collision/EnemyAttack/EnemyAttackCollision.h"
#include "../Collision/NinjaAttack/DashAttackCollision.h"
#include "../Collision/Field/FieldCollision.h"
#include "../Collision/Debug/CollisionDebug.h"

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

    EnemyManager m_enemyManager;

    DashAttackCollision m_dashAttackCollision;

    bool m_isTouchEnemy;

    // PlayerçUåÇîªíË
    PlayerAttackCollision m_playerAttackCollision;

    // PlayerÇÃéËó†åï
    NinjaAttack m_playerAttack;

    // ï`âÊä«óù
    DrawManager m_drawManager;
};