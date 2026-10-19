#include "FieldCollision.h"

#include "../../Ninja/Ninja.h"
#include "../../Field/Field.h"

bool FieldCollision::CheckWall(const Ninja& ninja, const Field& field)
{
    VECTOR ninjaPos = ninja.GetPosition();
    VECTOR wallPos = field.GetWallPosition();

    float halfWidth = WALL_WIDTH * 0.5f;
    float halfHeight = WALL_HEIGHT * 0.5f;
    float halfDepth = WALL_DEPTH * 0.5f;

    // X•ûŒü‚Ì”»’è
    if (ninjaPos.x < wallPos.x - halfWidth)
    {
        return false;
    }

    if (ninjaPos.x > wallPos.x + halfWidth)
    {
        return false;
    }

    // Y•ûŒü‚Ì”»’è
    if (ninjaPos.y < wallPos.y - halfHeight)
    {
        return false;
    }

    if (ninjaPos.y > wallPos.y + halfHeight)
    {
        return false;
    }

    // Z•ûŒü‚Ì”»’è
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
        wallPos.z - halfDepth);

    VECTOR pos2 = VGet(
        wallPos.x + halfWidth,
        wallPos.y + halfHeight,
        wallPos.z + halfDepth);

    DrawCube3D(pos1,pos2, 0,
        GetColor(255, 0, 0),
        GetColor(255, 0, 0));
}

void FieldCollision::ResolveWallCollision(Ninja& ninja,const Field& field,VECTOR oldNinjaPos)
{
    // •Ç‚É“ü‚Á‚½‚çˆÚ“®‘O‚ÌˆÊ’u‚É–ß‚·
    ninja.SetPosition(oldNinjaPos);

    // Ninja‚ª‹ó’†‚É‚¢‚éê‡
    if (ninja.IsJump())
    {
        VECTOR ninjaPos = ninja.GetPosition();
        VECTOR wallPos = field.GetWallPosition();

        VECTOR wallKickDirection = VGet(0.0f, 0.0f, 0.0f);

        // •Ç‚ªNinja‚Ì‰E‘¤‚É‚ ‚éê‡
        if (wallPos.x > ninjaPos.x)
        {
            wallKickDirection.x = -1.0f;
        }
        else
        {
            wallKickDirection.x = 1.0f;
        }

        ninja.SetWallContact(wallKickDirection);
    }
}