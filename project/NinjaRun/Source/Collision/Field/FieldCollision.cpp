#include "FieldCollision.h"

#include "../../Ninja/Ninja.h"
#include "../../Field/Field.h"
#include "../../Field/MoveFloor.h"

#include <cmath>


bool FieldCollision::CheckGroundAt(
    int modelHandle,
    VECTOR ninjaPos,
    float& groundY) const
{
    if (modelHandle < 0)
    {
        return false;
    }

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

    if (result.Normal.y < MIN_GROUND_NORMAL_Y)
    {
        return false;
    }

    groundY = result.HitPosition.y;

    return true;
}

bool FieldCollision::CheckGround(
    const Ninja& ninja,
    const Field& field,
    float& groundY)
{
    return CheckGroundAt(
        field.GetModelHandle(),
        ninja.GetPosition(),
        groundY);
}


void FieldCollision::ResolveGroundCollision(
    Ninja& ninja,
    const Field& field,
    const MoveFloor& moveFloor,
    VECTOR oldNinjaPos)
{
    VECTOR ninjaPos = ninja.GetPosition();

    const bool wasOnMoveFloor = m_onMoveFloor;
    const bool isGrounded = ninja.IsGrounded();

    VECTOR floorMove = moveFloor.GetMoveAmount();

    // 前フレームで移動床に乗っていて、
    // ジャンプしていなければ床の移動量を反映
    if (wasOnMoveFloor && isGrounded)
    {
        ninjaPos = VAdd(ninjaPos, floorMove);
        ninja.SetPosition(ninjaPos);
    }

    float fieldY = 0.0f;
    float moveFloorY = 0.0f;

    bool hasField = CheckGroundAt(
        field.GetModelHandle(),
        ninjaPos,
        fieldY);

    bool hasMoveFloor = CheckGroundAt(
        moveFloor.GetModelHandle(),
        ninjaPos,
        moveFloorY);

    bool foundGround = false;
    bool selectedMoveFloor = false;
    float selectedGroundY = 0.0f;

    // 接地中は、現在位置から小さな段差の範囲で
    // 追従できる床を探す
    if (isGrounded)
    {
        if (hasField)
        {
            float difference = fieldY - ninjaPos.y;

            if (fabsf(difference) <= GROUND_STEP_HEIGHT)
            {
                foundGround = true;
                selectedGroundY = fieldY;
            }
        }

        if (hasMoveFloor)
        {
            float difference = moveFloorY - ninjaPos.y;

            if (fabsf(difference) <= GROUND_STEP_HEIGHT)
            {
                if (!foundGround ||
                    moveFloorY > selectedGroundY)
                {
                    foundGround = true;
                    selectedMoveFloor = true;
                    selectedGroundY = moveFloorY;
                }
            }
        }
    }
    else
    {
        // 空中から通常床へ着地
        if (hasField &&
            oldNinjaPos.y >= fieldY &&
            ninjaPos.y <= fieldY)
        {
            foundGround = true;
            selectedGroundY = fieldY;
        }

        // 空中から移動床へ着地
        // 床の前フレームの高さも考慮する
        float previousMoveFloorY =
            moveFloorY - floorMove.y;

        if (hasMoveFloor &&
            oldNinjaPos.y >= previousMoveFloorY &&
            ninjaPos.y <= moveFloorY)
        {
            if (!foundGround ||
                moveFloorY > selectedGroundY)
            {
                foundGround = true;
                selectedMoveFloor = true;
                selectedGroundY = moveFloorY;
            }
        }
    }

    if (!foundGround)
    {
        m_onMoveFloor = false;
        ninja.SetAirborne();
        return;
    }

    ninja.ResolveGroundCollision(selectedGroundY);

    m_onMoveFloor =
        selectedMoveFloor && ninja.IsGrounded();
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

