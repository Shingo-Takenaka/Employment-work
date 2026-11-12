
#pragma once

#include "../Scene/SceneBase.h"
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

class PlayScene : public SceneBase
{
public:
    PlayScene();
    ~PlayScene() override;

    void Init() override;
    void Load() override;
    void Start() override;
    void Update() override;
    void Draw() override;

private:
    Camera m_camera;

    Field m_field;

    Ninja m_ninja;

    EnemyManager m_enemyManager;

    DashAttackCollision m_dashAttackCollision;
    FieldCollision m_fieldCollision;

    bool m_isTouchEnemy;

    // PlayerçUåÇîªíË
    PlayerAttackCollision m_playerAttackCollision;

    // PlayerÇÃéËó†åï
    NinjaAttack m_playerAttack;

    // ï`âÊä«óù
    DrawManager m_drawManager;
};
