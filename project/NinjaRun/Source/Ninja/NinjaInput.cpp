#include "Ninja.h"

#include "../Input/Input.h"
#include <math.h>

void Ninja::UpdateInput(
    bool& isMove,
    bool& jump,
    bool& slash,
    float& moveX,
    float& moveZ)
{

    isMove = false;
    jump = false;
    slash = false;

    moveX = 0.0f;
    moveZ = 0.0f;

    // キーボード入力
    if (Input::IsPress(KEY_INPUT_A))
    {
        moveX -= 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_D))
    {
        moveX += 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_W))
    {
        moveZ += 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_S))
    {
        moveZ -= 1.0f;
    }

    if (Input::IsTrigger(KEY_INPUT_SPACE))
    {
        jump = true;
    }

    if (Input::IsTrigger(KEY_INPUT_M))
    {
        slash = true;
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

    // パッドボタン
    // Aボタン
    if (Input::IsPadTrigger(0))
    {
        jump = true;
    }

    // Xボタン
    if (Input::IsPadTrigger(2))
    {
        slash = true;
    }

    // スティック優先
    if (stickX != 0.0f || stickY != 0.0f)
    {
        moveX = stickX;
        moveZ = -stickY;
    }

    // 斜め移動補正
    float length =
        sqrtf(moveX * moveX + moveZ * moveZ);

    if (length > 1.0f)
    {
        moveX /= length;
        moveZ /= length;
    }

    // 移動判定
    if (moveX != 0.0f || moveZ != 0.0f)
    {
        isMove = true;
    }
}