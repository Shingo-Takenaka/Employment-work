#include "TouchEnemyDebug.h"

#include "../Ninja/Ninja.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"
#include "TouchEnemy.h"

void TouchEnemyDebug::Draw(
    const Ninja& ninja,
    const NormalEnemy& enemy)
{
    VECTOR ninjaPos =
        ninja.GetPosition();

    // 現在のEnemyの座標を基準にする
    VECTOR enemyPos =
        enemy.GetPosition();

    // 当たり判定の幅
    const float width =
        TouchEnemy::GetTouchWidth();

    // 当たり判定の奥行き
    const float depth =
        TouchEnemy::GetTouchDepth();

    // 当たり判定の高さ
    const float height =
        TouchEnemy::GetTouchHeight();

    // Ninjaの判定表示
    VECTOR ninjaTopLeft =
        VGet(
            ninjaPos.x - width,
            ninjaPos.y + height,
            ninjaPos.z);

    VECTOR ninjaTopRight =
        VGet(
            ninjaPos.x + width,
            ninjaPos.y + height,
            ninjaPos.z);

    VECTOR ninjaBottomLeft =
        VGet(
            ninjaPos.x - width,
            ninjaPos.y,
            ninjaPos.z);

    VECTOR ninjaBottomRight =
        VGet(
            ninjaPos.x + width,
            ninjaPos.y,
            ninjaPos.z);

    int ninjaColor =
        GetColor(0, 255, 0);

    DrawLine3D(
        ninjaTopLeft,
        ninjaTopRight,
        ninjaColor);

    DrawLine3D(
        ninjaBottomLeft,
        ninjaBottomRight,
        ninjaColor);

    DrawLine3D(
        ninjaTopLeft,
        ninjaBottomLeft,
        ninjaColor);

    DrawLine3D(
        ninjaTopRight,
        ninjaBottomRight,
        ninjaColor);

    // Enemyの判定表示
    VECTOR enemyTopLeft =
        VGet(
            enemyPos.x - width,
            enemyPos.y + height,
            enemyPos.z);

    VECTOR enemyTopRight =
        VGet(
            enemyPos.x + width,
            enemyPos.y + height,
            enemyPos.z);

    VECTOR enemyBottomLeft =
        VGet(
            enemyPos.x - width,
            enemyPos.y,
            enemyPos.z);

    VECTOR enemyBottomRight =
        VGet(
            enemyPos.x + width,
            enemyPos.y,
            enemyPos.z);

    int enemyColor =
        GetColor(0, 150, 255);

    DrawLine3D(
        enemyTopLeft,
        enemyTopRight,
        enemyColor);

    DrawLine3D(
        enemyBottomLeft,
        enemyBottomRight,
        enemyColor);

    DrawLine3D(
        enemyTopLeft,
        enemyBottomLeft,
        enemyColor);

    DrawLine3D(
        enemyTopRight,
        enemyBottomRight,
        enemyColor);
}