
#pragma once

#include "DxLib.h"

class Ninja;
class Field;

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
        VECTOR oldNinjaPos);

    // ï«Ç∆ÇÃìñÇΩÇËîªíË
    void ResolveWallCollision(
        Ninja& ninja,
        const Field& field,
        VECTOR oldNinjaPos);

    void DrawDebug(const Field& field);

private:
    static constexpr float MIN_GROUND_NORMAL_Y = 0.7f;
    static constexpr float GROUND_STEP_HEIGHT = 2.0f;

    // ï«îªíËÇÃê›íË
    static constexpr float WALL_RADIUS = 1.5f;
    static constexpr float MAX_WALL_NORMAL_Y = 0.3f;
};
