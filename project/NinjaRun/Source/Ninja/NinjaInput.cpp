#include "Ninja.h"

#include "../Input/Input.h"
#include <math.h>

void Ninja::UpdateInput()
{
    // 初期化
    m_input.moveX = 0.0f;
    m_input.moveZ = 0.0f;

    m_input.isMove = false;

    m_input.jump = false;
    m_input.slash = false;
    m_input.guard = false;
    m_input.shoot = false;
    m_input.dash = false;

    // キーボード入力
    // 移動
    bool pressA = Input::IsPress(KEY_INPUT_A);
    bool pressD = Input::IsPress(KEY_INPUT_D);
    bool pressW = Input::IsPress(KEY_INPUT_W);
    bool pressS = Input::IsPress(KEY_INPUT_S);

    // Aを新しく押した
    if (pressA && !m_prevA)
    {
        m_lastHorizontalInput = -1;
    }

    // Dを新しく押した
    if (pressD && !m_prevD)
    {
        m_lastHorizontalInput = 1;
    }

    // Wを新しく押した
    if (pressW && !m_prevW)
    {
        m_lastVerticalInput = 1;
    }

    // Sを新しく押した
    if (pressS && !m_prevS)
    {
        m_lastVerticalInput = -1;
    }

    // AとDの入力
    if (pressA && pressD)
    {
        // 後から押した方向を優先
        if (m_lastHorizontalInput == -1)
        {
            m_input.moveX = -1.0f;
        }
        else if (m_lastHorizontalInput == 1)
        {
            m_input.moveX = 1.0f;
        }
    }
    else if (pressA)
    {
        m_input.moveX = -1.0f;
    }
    else if (pressD)
    {
        m_input.moveX = 1.0f;
    }
    else
    {
        m_lastHorizontalInput = 0;
    }

    // WとSの入力
    if (pressW && pressS)
    {
        // 後から押した方向を優先
        if (m_lastVerticalInput == 1)
        {
            m_input.moveZ = 1.0f;
        }
        else if (m_lastVerticalInput == -1)
        {
            m_input.moveZ = -1.0f;
        }
    }
    else if (pressW)
    {
        m_input.moveZ = 1.0f;
    }
    else if (pressS)
    {
        m_input.moveZ = -1.0f;
    }
    else
    {
        m_lastVerticalInput = 0;
    }

    // 次のフレーム用にA/Dの状態を保存
    m_prevA = pressA;
    m_prevD = pressD;

    // 次のフレーム用にW/Sの状態を保存
    m_prevW = pressW;
    m_prevS = pressS;

    // ジャンプ
    if (Input::IsTrigger(KEY_INPUT_SPACE))
    {
        m_input.jump = true;
    }

    // 攻撃
    if (Input::IsTrigger(KEY_INPUT_M))
    {
        m_input.slash = true;
    }

    // ガード
    if (Input::IsPress(KEY_INPUT_K))
    {
        m_input.guard = true;
    }

    // 手裏剣
    if (Input::IsTrigger(KEY_INPUT_N))
    {
        m_input.shoot = true;
    }

    // ダッシュ
    if (Input::IsPress(KEY_INPUT_L))
    {
        m_input.dash = true;
    }

    // コントローラー入力
#pragma region 入力関数一覧
/*
IsPadTrigger(N)
A:0
B:1
X:2
Y:3
LB:4
RB:5
BACK/SELECT:6
START:7
L3:8(スティック押し込み)
R3:9(スティック押し込み)
LT:10
RT:11
*/
#pragma endregion

    DINPUT_JOYSTATE joyState;

    GetJoypadDirectInputState(
        DX_INPUT_PAD1,
        &joyState);

    float stickX = joyState.X / 1000.0f;
    float stickY = joyState.Y / 1000.0f;

    // デッドゾーン
    const float DEAD_ZONE = 0.2f;

    if (fabsf(stickX) < DEAD_ZONE)
    {
        stickX = 0.0f;
    }

    if (fabsf(stickY) < DEAD_ZONE)
    {
        stickY = 0.0f;
    }

    // ゲームパッド
    // Aボタン
    if (Input::IsPadTrigger(0))
    {
        m_input.jump = true;
    }

    // Bボタン
    if (Input::IsPadPress(1))
    {
        m_input.dash = true;
    }

    // Xボタン
    if (Input::IsPadTrigger(2))
    {
        m_input.slash = true;
    }

    // Yボタン
    if (Input::IsPadTrigger(3))
    {
        m_input.shoot = true;
    }

    // RBボタン
    if (joyState.Buttons[5])
    {
        m_input.guard = true;
    }

    // 左スティック
    // キーボードより優先
    if (stickX != 0.0f || stickY != 0.0f)
    {
        m_input.moveX = stickX;
        m_input.moveZ = -stickY;
    }

    // 斜め移動補正
    float length =
        sqrtf(
            m_input.moveX * m_input.moveX +
            m_input.moveZ * m_input.moveZ);

    if (length > 1.0f)
    {
        m_input.moveX /= length;
        m_input.moveZ /= length;
    }

    // 移動判定
    if (m_input.moveX != 0.0f ||
        m_input.moveZ != 0.0f)
    {
        m_input.isMove = true;
    }
}