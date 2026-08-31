#include "TouchEnemy.h"

#include "../Ninja/Ninja.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"

#include <math.h>

// X方向の判定範囲
float TouchEnemy::GetTouchWidth()
{
    return 8.0f * 0.6f;
}

// Z方向の判定範囲
float TouchEnemy::GetTouchDepth()
{
    return 4.0f;
}

bool TouchEnemy::Check(
    const Ninja& ninja,
    const NormalEnemy& enemy)
{
    VECTOR ninjaPos =
        ninja.GetPosition();

    VECTOR enemyPos =
        enemy.GetPosition();

    // X方向の中心間距離
    float dx =
        fabsf(ninjaPos.x - enemyPos.x);

    // Z方向の中心間距離
    float dz =
        fabsf(ninjaPos.z - enemyPos.z);

    // PlayerとEnemyそれぞれの判定幅を考慮
    float width =
        GetTouchWidth() * 2.0f;

    // PlayerとEnemyそれぞれの奥行き判定を考慮
    float depth =
        GetTouchDepth() * 2.0f;

    // X方向・Z方向の判定範囲が重なっていれば接触
    return
        dx <= width &&
        dz <= depth;
}