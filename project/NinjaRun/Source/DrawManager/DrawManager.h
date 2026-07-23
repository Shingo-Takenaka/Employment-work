#pragma once

#include <vector>
#include <functional>
#include "DxLib.h"

// 描画情報
struct DrawObject
{
    VECTOR position;

    // ソート用の値
    float sortValue;

    std::function<void()> draw;
};

class DrawManager
{
public:

    DrawManager();
    ~DrawManager();

    // 登録
    void Add(
        VECTOR position,
        float sortValue,
        std::function<void()> draw);

    // 描画
    void Draw();

    // リスト初期化
    void Clear();

private:

    std::vector<DrawObject> m_drawList;
};