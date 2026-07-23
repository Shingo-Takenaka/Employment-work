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

    // キーボード入力
    // 移動
    if (Input::IsPress(KEY_INPUT_A))
    {
        m_input.moveX -= 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_D))
    {
        m_input.moveX += 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_W))
    {
        m_input.moveZ += 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_S))
    {
        m_input.moveZ -= 1.0f;
    }

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
    if (Input::IsPress(KEY_INPUT_N))
    {
        m_input.guard = true;
    }

    // コントローラー入力
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

    // Xボタン
    if (Input::IsPadTrigger(2))
    {
        m_input.slash = true;
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