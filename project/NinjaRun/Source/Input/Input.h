#pragma once
#include "DxLib.h"

class Input
{
public:
    // キーの状態を更新
    static void Update();

    // 指定したキーが押し続けられている
    static bool IsPress(int keyCode);

    // 指定したキーが押された瞬間
    static bool IsTrigger(int keyCode);

    // 指定したキーが離された瞬間
    static bool IsRelease(int keyCode);

    // 指定したボタンが押し続けられている
    static bool IsPadPress(int button);

    // 指定したボタンが押された瞬間
    static bool IsPadTrigger(int button);

    // 指定したボタンが離された瞬間
    static bool IsPadRelease(int button);

    // LTを押している
    static bool IsLTPress();

    // LTを押した瞬間
    static bool IsLTTrigger();

    // RTを押している
    static bool IsRTPress();

    // RTを押した瞬間
    static bool IsRTTrigger();

    // LTの値 0.0～1.0
    static float GetLT();

    // RTの値 0.0～1.0
    static float GetRT();

    // 左スティック X方向 -1.0～1.0
    static float GetLeftStickX();

    // 左スティック Y方向 -1.0～1.0
    static float GetLeftStickY();

    // 右スティック X方向 -1.0～1.0
    static float GetRightStickX();

    // 右スティック Y方向 -1.0～1.0
    static float GetRightStickY();

    // L3
    static bool IsL3Press();
    static bool IsL3Trigger();

    // R3
    static bool IsR3Press();
    static bool IsR3Trigger();

    // 十字キー
    static bool IsDPadUp();
    static bool IsDPadDown();
    static bool IsDPadLeft();
    static bool IsDPadRight();

private:
    // キーボード
    static char m_currentKey[256];
    static char m_prevKey[256];

    // ゲームパッド
    static DINPUT_JOYSTATE m_currentPad;
    static DINPUT_JOYSTATE m_prevPad;

    // XInput
    static XINPUT_STATE m_currentXPad;
    static XINPUT_STATE m_prevXPad;

    static bool m_xInputAvailable;
};