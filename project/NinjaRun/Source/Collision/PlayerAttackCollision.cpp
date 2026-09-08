#include "PlayerAttackCollision.h"

#include "../Ninja/Ninja.h"
#include "../Enemy/NormalEnemy/NormalEnemy.h"

#include <math.h>

PlayerAttackCollision::PlayerAttackCollision()
{
}

// Playerの攻撃とEnemyが当たっているか
bool PlayerAttackCollision::Check(
    const Ninja& ninja,
    const NormalEnemy& enemy)
{
    // 攻撃中でなければ当たり判定なし
    if (!ninja.IsSlash())
    {
        return false;
    }

    VECTOR ninjaPos =
        ninja.GetPosition();

    VECTOR enemyPos =
        enemy.GetPosition();

    // PlayerAttackから攻撃範囲を取得
    float attackWidth =
        m_playerAttack.GetAttackWidth();

    float attackDepth =
        m_playerAttack.GetAttackDepth();

    float attackHeight =
        m_playerAttack.GetAttackHeight();

    // X方向
    float minX;
    float maxX;

    if (!ninja.IsReverseX())
    {
        // 右向き
        minX = ninjaPos.x;
        maxX = ninjaPos.x + attackWidth;
    }
    else
    {
        // 左向き
        minX = ninjaPos.x - attackWidth;
        maxX = ninjaPos.x;
    }

    // X方向の判定
    if (enemyPos.x < minX ||
        enemyPos.x > maxX)
    {
        return false;
    }

    // Z方向
    float dz =
        fabsf(enemyPos.z - ninjaPos.z);

    if (dz > attackDepth)
    {
        return false;
    }

    // Y方向
    float dy =
        fabsf(enemyPos.y - (ninjaPos.y - 5.0f));

    if (dy > attackHeight)
    {
        return false;
    }

    return true;
}

// 攻撃範囲を表示
void PlayerAttackCollision::DrawDebug(
    const Ninja& ninja)
{
    // 攻撃中でなければ表示しない
    if (!ninja.IsSlash())
    {
        return;
    }

    VECTOR ninjaPos =
        ninja.GetPosition();

    float attackWidth =
        m_playerAttack.GetAttackWidth();

    float attackDepth =
        m_playerAttack.GetAttackDepth();

    float attackHeight =
        m_playerAttack.GetAttackHeight();

    // X方向
    float minX;
    float maxX;

    if (!ninja.IsReverseX())
    {
        // 右向き
        minX = ninjaPos.x;
        maxX = ninjaPos.x + attackWidth;
    }
    else
    {
        // 左向き
        minX = ninjaPos.x - attackWidth;
        maxX = ninjaPos.x;
    }

    // Z方向
    float minZ =
        ninjaPos.z - attackDepth;

    float maxZ =
        ninjaPos.z + attackDepth;

    // Y方向
    float attackOffsetY = 5.0f;

    float minY =
        ninjaPos.y - attackOffsetY - attackHeight;

    float maxY =
        ninjaPos.y - attackOffsetY + attackHeight;

    // デバッグ表示色
    unsigned int color =
        GetColor(255, 0, 0);

    // 下側
    DrawLine3D(
        VGet(minX, minY, minZ),
        VGet(maxX, minY, minZ),
        color);

    DrawLine3D(
        VGet(minX, minY, maxZ),
        VGet(maxX, minY, maxZ),
        color);

    DrawLine3D(
        VGet(minX, minY, minZ),
        VGet(minX, minY, maxZ),
        color);

    DrawLine3D(
        VGet(maxX, minY, minZ),
        VGet(maxX, minY, maxZ),
        color);

    // 上側
    DrawLine3D(
        VGet(minX, maxY, minZ),
        VGet(maxX, maxY, minZ),
        color);

    DrawLine3D(
        VGet(minX, maxY, maxZ),
        VGet(maxX, maxY, maxZ),
        color);

    DrawLine3D(
        VGet(minX, maxY, minZ),
        VGet(minX, maxY, maxZ),
        color);

    DrawLine3D(
        VGet(maxX, maxY, minZ),
        VGet(maxX, maxY, maxZ),
        color);

    // 縦
    DrawLine3D(
        VGet(minX, minY, minZ),
        VGet(minX, maxY, minZ),
        color);

    DrawLine3D(
        VGet(maxX, minY, minZ),
        VGet(maxX, maxY, minZ),
        color);

    DrawLine3D(
        VGet(minX, minY, maxZ),
        VGet(minX, maxY, maxZ),
        color);

    DrawLine3D(
        VGet(maxX, minY, maxZ),
        VGet(maxX, maxY, maxZ),
        color);
}