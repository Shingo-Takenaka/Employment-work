
#pragma once

#include "DxLib.h"

class Ninja;
class Field;
class MoveFloor;

class FieldCollision
{
public:
    bool CheckGround(
        const Ninja& ninja,
        const Field& field,
        float& groundY);

    void ResolveGroundCollision(
        Ninja& ninja,
        const Field& field,
        const MoveFloor& moveFloor,
        VECTOR oldNinjaPos);

    void ResolveWallCollision(
        Ninja& ninja,
        const Field& field,
        VECTOR oldNinjaPos);

    void DrawDebug(const Field& field);

private:
    bool CheckGroundAt(
        int modelHandle,
        VECTOR ninjaPos,
        float& groundY) const;

    bool m_onMoveFloor = false;

    static constexpr float MIN_GROUND_NORMAL_Y = 0.7f;
    static constexpr float GROUND_STEP_HEIGHT = 2.0f;

    static constexpr float WALL_RADIUS = 1.5f;
    static constexpr float MAX_WALL_NORMAL_Y = 0.3f;
};
