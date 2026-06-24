#pragma once
#include "DxLib.h"

class Ninja
{
public:
    Ninja();
    ~Ninja();

    void Update();
    void Draw();

private:
    int m_handle;       // 画像ハンドル
    VECTOR m_pos;       // 3D空間上の位置
    float m_size;       // 描画サイズ
    float m_moveSpeed;  // 移動速度
};