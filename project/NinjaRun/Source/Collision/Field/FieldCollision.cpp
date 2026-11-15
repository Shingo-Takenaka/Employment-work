
#include "FieldCollision.h"

#include "../../Ninja/Ninja.h"
#include "../../Field/Field.h"

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
