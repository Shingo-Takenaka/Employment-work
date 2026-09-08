#include "../DrawManager/DrawManager.h"
#include "../Camera/Camera.h"
#include "../Field/Field.h"
#include "../Ninja/Ninja.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"
#include "../Collision/TouchEnemy.h"
#include "../Collision/PlayerAttackCollision.h"

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

    bool m_isTouchEnemy;

    // PlayerçUåÇîªíË
    PlayerAttackCollision m_playerAttackCollision;

    // ï`âÊä«óù
    DrawManager m_drawManager;
};