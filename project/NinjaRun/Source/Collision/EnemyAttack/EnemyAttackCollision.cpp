#include "EnemyAttackCollision.h"
#include "../../Ninja/Ninja.h"
#include "../CollisionSetting.h"

#include <math.h>

bool EnemyAttackCollision::Check(const Ninja& ninja, VECTOR start, VECTOR target)
{
    VECTOR playerPos = ninja.GetPosition();

    float dx = target.x - start.x;

    float dz = target.z - start.z;

    float length = sqrtf(dx * dx + dz * dz);

    if (length <= 0.001f)
    {
        return false;
    }

    float dirX = dx / length;

    float dirZ = dz / length;

    float sideX = -dirZ;

    float sideZ = dirX;

    float playerX = playerPos.x - start.x;

    float playerY = playerPos.y - start.y;

    float playerZ = playerPos.z - start.z;

    float forwardDistance = playerX * dirX + playerZ * dirZ;

    float sideDistance = playerX * sideX + playerZ * sideZ;

    // Playerの半分のサイズ
    float playerHalfWidth = CollisionSettings::ENEMY_ATTACK_PLAYER_WIDTH * 0.5f;

    float playerHalfHeight = CollisionSettings::ENEMY_ATTACK_PLAYER_HEIGHT * 0.5f;

    float playerHalfDepth = CollisionSettings::ENEMY_ATTACK_PLAYER_DEPTH * 0.5f;

    // Beamの半分のサイズ
    float beamHalfWidth = CollisionSettings::ENEMY_ATTACK_WIDTH * 0.5f;

    float beamHalfHeight = CollisionSettings::ENEMY_ATTACK_HEIGHT * 0.5f;

    // Beamの前後方向の判定
    if (forwardDistance + playerHalfDepth < 0.0f ||
        forwardDistance - playerHalfDepth > length)
    {
        return false;
    }

    // Beamの横方向の判定
    if (fabsf(sideDistance) > beamHalfWidth + playerHalfWidth)
    {
        return false;
    }

    // Beamの高さ方向の判定
    if (fabsf(playerY) > beamHalfHeight + playerHalfHeight)
    {
        return false;
    }

    return true;
}

void EnemyAttackCollision::DrawDebug(VECTOR start, VECTOR target)
{
    // XZ平面上での攻撃方向
    float dx = target.x - start.x;

    float dz = target.z - start.z;

    float length = sqrtf(dx * dx + dz * dz);

    if (length <= 0.001f)
    {
        return;
    }

    // 攻撃方向を正規化
    float dirX = dx / length;

    float dirZ = dz / length;

    // 攻撃方向に対して横向きの方向
    VECTOR side = VGet(-dirZ, 0.0f, dirX);

    // 上方向
    VECTOR up = VGet(0.0f, 1.0f, 0.0f);

    float halfWidth = CollisionSettings::ENEMY_ATTACK_WIDTH * 0.5f;

    float halfHeight = CollisionSettings::ENEMY_ATTACK_HEIGHT * 0.5f;

    // 開始地点の4頂点
    VECTOR startTopRight = VAdd(VAdd(start, VScale(side, halfWidth)), VScale(up, halfHeight));

    VECTOR startTopLeft = VAdd(VAdd(start, VScale(side, -halfWidth)), VScale(up, halfHeight));

    VECTOR startBottomRight = VAdd(VAdd(start, VScale(side, halfWidth)), VScale(up, -halfHeight));

    VECTOR startBottomLeft = VAdd(VAdd(start, VScale(side, -halfWidth)), VScale(up, -halfHeight));

    // 終了地点の4頂点
    VECTOR targetTopRight = VAdd(VAdd(target, VScale(side, halfWidth)), VScale(up, halfHeight));

    VECTOR targetTopLeft = VAdd(VAdd(target, VScale(side, -halfWidth)), VScale(up, halfHeight));

    VECTOR targetBottomRight = VAdd(VAdd(target, VScale(side, halfWidth)), VScale(up, -halfHeight));

    VECTOR targetBottomLeft = VAdd(VAdd(target, VScale(side, -halfWidth)), VScale(up, -halfHeight));

    // デバッグ表示の色
    int debugColor = GetColor(255, 0, 0);

    // 開始側
    DrawLine3D(startTopRight, startTopLeft, debugColor);

    DrawLine3D(startTopLeft, startBottomLeft, debugColor);

    DrawLine3D(startBottomLeft, startBottomRight, debugColor);

    DrawLine3D(startBottomRight, startTopRight, debugColor);

    // 終了側
    DrawLine3D(targetTopRight, targetTopLeft, debugColor);

    DrawLine3D(targetTopLeft, targetBottomLeft, debugColor);

    DrawLine3D(targetBottomLeft, targetBottomRight, debugColor);

    DrawLine3D(targetBottomRight, targetTopRight, debugColor);

    // 側面
    DrawLine3D(startTopRight, targetTopRight, debugColor);

    DrawLine3D(startTopLeft, targetTopLeft, debugColor);

    DrawLine3D(startBottomRight, targetBottomRight, debugColor);

    DrawLine3D(startBottomLeft, targetBottomLeft, debugColor);
}