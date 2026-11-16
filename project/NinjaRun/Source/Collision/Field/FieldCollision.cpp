
#include "FieldCollision.h"

#include "../../Ninja/Ninja.h"
#include "../../Field/Field.h"
#include <cmath>

bool FieldCollision::CheckGround(
    const Ninja& ninja,
    const Field& field,
    float& groundY)
{
    int modelHandle = field.GetModelHandle();

    if (modelHandle < 0)
    {
        return false;
    }

    VECTOR ninjaPos = ninja.GetPosition();

    VECTOR startPos = VGet(
        ninjaPos.x,
        ninjaPos.y + 10.0f,
        ninjaPos.z);

    VECTOR endPos = VGet(
        ninjaPos.x,
        ninjaPos.y - 20.0f,
        ninjaPos.z);

    MV1_COLL_RESULT_POLY result =
        MV1CollCheck_Line(
            modelHandle,
            -1,
            startPos,
            endPos);

    if (result.HitFlag == 0)
    {
        return false;
    }

    // 地面として扱える傾斜か確認
    if (result.Normal.y < MIN_GROUND_NORMAL_Y)
    {
        return false;
    }

    groundY = result.HitPosition.y;

    return true;
}

void FieldCollision::ResolveGroundCollision(
    Ninja& ninja,
    const Field& field,
    VECTOR oldNinjaPos)
{
    float groundY = 0.0f;

    if (!CheckGround(ninja, field, groundY))
    {
        ninja.SetAirborne();
        return;
    }

    VECTOR ninjaPos = ninja.GetPosition();

    // 接地中の坂道追従
    if (ninja.IsGrounded())
    {
        float heightDifference = groundY - ninjaPos.y;

        if (heightDifference <= GROUND_STEP_HEIGHT &&
            heightDifference >= -GROUND_STEP_HEIGHT)
        {
            ninja.ResolveGroundCollision(groundY);
            return;
        }

        ninja.SetAirborne();
        return;
    }

    // 空中からの着地
    // 移動前の足元が地面以上で、
    // 移動後の足元が地面以下の場合
    if (oldNinjaPos.y >= groundY &&
        ninjaPos.y <= groundY)
    {
        ninja.ResolveGroundCollision(groundY);
    }
}

void FieldCollision::DrawDebug(
    const Field& field)
{
    // 現段階ではデバッグ描画なし
}


void FieldCollision::ResolveWallCollision(
    Ninja& ninja,
    const Field& field,
    VECTOR oldNinjaPos)
{
    int modelHandle = field.GetModelHandle();

    if (modelHandle < 0)
    {
        return;
    }

    VECTOR ninjaPos = ninja.GetPosition();
    VECTOR resolvedPos = ninjaPos;

    const float wallCheckHeights[] =
    {
        2.0f,
        5.0f,
        8.0f
    };

    // X方向の壁判定
    float moveX = ninjaPos.x - oldNinjaPos.x;

    if (fabsf(moveX) > 0.0001f)
    {
        float directionX = moveX > 0.0f ? 1.0f : -1.0f;
        float nearestDistance = fabsf(moveX);
        bool hitWall = false;
        VECTOR wallNormal = VGet(0.0f, 0.0f, 0.0f);

        for (float height : wallCheckHeights)
        {
            VECTOR startPos = VGet(
                oldNinjaPos.x,
                oldNinjaPos.y + height,
                oldNinjaPos.z);

            VECTOR endPos = VGet(
                ninjaPos.x + directionX * WALL_RADIUS,
                oldNinjaPos.y + height,
                oldNinjaPos.z);

            MV1_COLL_RESULT_POLY result =
                MV1CollCheck_Line(
                    modelHandle,
                    -1,
                    startPos,
                    endPos);

            if (result.HitFlag == 0)
            {
                continue;
            }

            // 床や坂道を除外
            if (fabsf(result.Normal.y) > MAX_WALL_NORMAL_Y)
            {
                continue;
            }

            // 進行方向に対して向かい合う壁だけを対象にする
            if (result.Normal.x * directionX >= -0.01f)
            {
                continue;
            }

            float distance =
                (result.HitPosition.x - oldNinjaPos.x)
                * directionX - WALL_RADIUS;

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                hitWall = true;
                wallNormal = result.Normal;
            }
        }

        if (hitWall)
        {
            if (nearestDistance < 0.0f)
            {
                nearestDistance = 0.0f;
            }

            resolvedPos.x =
                oldNinjaPos.x + directionX * nearestDistance;

            ninja.SetWallContact(wallNormal);
        }
    }

    // Z方向の壁判定
    float moveZ = ninjaPos.z - oldNinjaPos.z;

    if (fabsf(moveZ) > 0.0001f)
    {
        float directionZ = moveZ > 0.0f ? 1.0f : -1.0f;
        float nearestDistance = fabsf(moveZ);
        bool hitWall = false;
        VECTOR wallNormal = VGet(0.0f, 0.0f, 0.0f);

        for (float height : wallCheckHeights)
        {
            VECTOR startPos = VGet(
                resolvedPos.x,
                oldNinjaPos.y + height,
                oldNinjaPos.z);

            VECTOR endPos = VGet(
                resolvedPos.x,
                oldNinjaPos.y + height,
                ninjaPos.z + directionZ * WALL_RADIUS);

            MV1_COLL_RESULT_POLY result =
                MV1CollCheck_Line(
                    modelHandle,
                    -1,
                    startPos,
                    endPos);

            if (result.HitFlag == 0)
            {
                continue;
            }

            if (fabsf(result.Normal.y) > MAX_WALL_NORMAL_Y)
            {
                continue;
            }

            if (result.Normal.z * directionZ >= -0.01f)
            {
                continue;
            }

            float distance =
                (result.HitPosition.z - oldNinjaPos.z)
                * directionZ - WALL_RADIUS;

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                hitWall = true;
                wallNormal = result.Normal;
            }
        }

        if (hitWall)
        {
            if (nearestDistance < 0.0f)
            {
                nearestDistance = 0.0f;
            }

            resolvedPos.z =
                oldNinjaPos.z + directionZ * nearestDistance;

            ninja.SetWallContact(wallNormal);
        }
    }

    ninja.SetPosition(resolvedPos);
}

