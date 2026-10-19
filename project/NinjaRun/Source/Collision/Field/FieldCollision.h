#pragma once

#include "DxLib.h"

class Ninja;
class Field;

class FieldCollision
{
public:
    // 壁との当たり判定
    bool CheckWall(const Ninja& ninja, const Field& field);

    // 壁との衝突処理
    void ResolveWallCollision(Ninja& ninja,const Field& field,VECTOR oldNinjaPos);

    // 壁の当たり判定をデバッグ表示
    void DrawDebug(const Field& field);

private:
    // 壁の当たり判定サイズ
    static constexpr float WALL_WIDTH = 5.0f;
    static constexpr float WALL_HEIGHT = 200.0f;
    static constexpr float WALL_DEPTH = 100.0f;
};