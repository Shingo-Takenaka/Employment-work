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

    VECTOR enemyPos =
        enemy.GetPosition();

    // 当たり判定サイズ
    const float width =
        TouchEnemy::GetTouchWidth();

    // キャラクターの中心高さ
    const float height = 5.0f;

    // Ninja
    // デバッグ表示の高さ位置
    const float debugOffsetY = -5.0f;

    // 上側
    VECTOR ninjaTopLeft =
        VGet(
            ninjaPos.x - width,
            ninjaPos.y + height + debugOffsetY,
            ninjaPos.z);

    VECTOR ninjaTopRight =
        VGet(
            ninjaPos.x + width,
            ninjaPos.y + height + debugOffsetY,
            ninjaPos.z);

    // 下側
    VECTOR ninjaBottomLeft =
        VGet(
            ninjaPos.x - width,
            ninjaPos.y - height + debugOffsetY,
            ninjaPos.z);

    VECTOR ninjaBottomRight =
        VGet(
            ninjaPos.x + width,
            ninjaPos.y - height + debugOffsetY,
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

    // Enemy
    VECTOR enemyTopLeft =
        VGet(
            enemyPos.x - width,
            enemyPos.y + height + debugOffsetY,
            enemyPos.z);

    VECTOR enemyTopRight =
        VGet(
            enemyPos.x + width,
            enemyPos.y + height + debugOffsetY,
            enemyPos.z);

    VECTOR enemyBottomLeft =
        VGet(
            enemyPos.x - width,
            enemyPos.y - height + debugOffsetY,
            enemyPos.z);

    VECTOR enemyBottomRight =
        VGet(
            enemyPos.x + width,
            enemyPos.y - height + debugOffsetY,
            enemyPos.z);

    // Enemy：青
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