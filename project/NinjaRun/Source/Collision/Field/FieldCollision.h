
#pragma once

#include "DxLib.h"

class Ninja;
class Field;

class FieldCollision
{
public:
    // 地面の高さを取得
    bool CheckGround(
        const Ninja& ninja,
        const Field& field,
        float& groundY);

    // 地面との衝突処理
    void ResolveGroundCollision(
        Ninja& ninja,
        const Field& field,
        VECTOR oldNinjaPos);

    // デバッグ描画
    void DrawDebug(const Field& field);

private:
    // 地面の最大傾斜
    static constexpr float MIN_GROUND_NORMAL_Y = 0.7f;

    // 地面への追従を許可する高さ
    static constexpr float GROUND_STEP_HEIGHT = 2.0f;
};
