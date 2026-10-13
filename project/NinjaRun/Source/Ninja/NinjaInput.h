#pragma once
#include "DxLib.h"

// 入力情報
struct NinjaInputState
{
    // 移動方向
    float moveX;
    float moveZ;

    // 行動系入力
    bool jump;
    bool dash;
    bool guard;
    bool isMove;

    // 攻撃系入力
    bool slash;
    bool dashCut;
    bool shoot;
};

class NinjaInput
{
public:
    NinjaInput();

    // 入力を取得する
    void Update();

    // 取得した入力情報
    const NinjaInputState& GetState() const;

private:
    // 入力情報
    NinjaInputState m_state;

    // 前フレームのキー状態
    bool m_prevA;
    bool m_prevD;
    bool m_prevW;
    bool m_prevS;
};
