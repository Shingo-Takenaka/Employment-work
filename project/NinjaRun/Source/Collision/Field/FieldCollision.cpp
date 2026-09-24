#include "FieldCollision.h"

#include "../../Ninja/Ninja.h"
#include "../../Field/Field.h"

bool FieldCollision::CheckWall(
    const Ninja& ninja,
    const Field& field)
{
    VECTOR ninjaPos = ninja.GetPosition();
    VECTOR wallPos = field.GetWallPosition();

    float halfWidth = WALL_WIDTH * 0.5f;
    float halfDepth = WALL_DEPTH * 0.5f;

    if (ninjaPos.x < wallPos.x - halfWidth)
    {
        return false;
    }

    if (ninjaPos.x > wallPos.x + halfWidth)
    {
        return false;
    }

    if (ninjaPos.z < wallPos.z - halfDepth)
    {
        return false;
    }

    if (ninjaPos.z > wallPos.z + halfDepth)
    {
        return false;
    }

    return true;
}

void FieldCollision::DrawDebug(
    const Field& field)
{
    VECTOR wallPos = field.GetWallPosition();

    float halfWidth = WALL_WIDTH * 0.5f;
    float halfHeight = WALL_HEIGHT * 0.5f;
    float halfDepth = WALL_DEPTH * 0.5f;

    VECTOR pos1 = VGet(
        wallPos.x - halfWidth,
        wallPos.y - halfHeight,
        wallPos.z - halfDepth
    );

    VECTOR pos2 = VGet(
        wallPos.x + halfWidth,
        wallPos.y + halfHeight,
        wallPos.z + halfDepth
    );

    DrawCube3D(
        pos1,
        pos2,
        0,
        GetColor(255, 0, 0),
        GetColor(255, 0, 0)
    );
}