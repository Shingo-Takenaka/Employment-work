#pragma once

#include "DxLib.h"

class Ninja;
class Field;

class FieldCollision
{
public:

    // Wallとの当たり判定
    static bool CheckWall(
        const Ninja& ninja,
        const Field& field
    );

    // Wallの当たり判定をデバッグ表示
    static void DrawDebug(
        const Field& field
    );

private:

    // Wallの当たり判定サイズ
    static constexpr float WALL_WIDTH = 100.0f;
    static constexpr float WALL_HEIGHT = 20.0f;
    static constexpr float WALL_DEPTH = 5.0f;
};