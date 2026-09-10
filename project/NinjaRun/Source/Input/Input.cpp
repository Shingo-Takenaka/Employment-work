#include "Input.h"

char Input::m_currentKey[256] = { 0 };
char Input::m_prevKey[256] = { 0 };

DINPUT_JOYSTATE Input::m_currentPad = { 0 };
DINPUT_JOYSTATE Input::m_prevPad = { 0 };

XINPUT_STATE Input::m_currentXPad = { 0 };
XINPUT_STATE Input::m_prevXPad = { 0 };

bool Input::m_xInputAvailable = false;

void Input::Update()
{
    // キーボード
    for (int i = 0; i < 256; i++)
    {
        m_prevKey[i] = m_currentKey[i];
    }

    GetHitKeyStateAll(m_currentKey);

    // ゲームパッド
    m_prevPad = m_currentPad;

    GetJoypadDirectInputState(
        DX_INPUT_PAD1,
        &m_currentPad
    );

    // XInput
    m_prevXPad = m_currentXPad;

    int result = GetJoypadXInputState(
        DX_INPUT_PAD1,
        &m_currentXPad
    );

    m_xInputAvailable = (result == 0);
}

bool Input::IsPress(int keyCode)
{
    return m_currentKey[keyCode] != 0;
}

bool Input::IsTrigger(int keyCode)
{
    return
        (m_currentKey[keyCode] != 0) &&
        (m_prevKey[keyCode] == 0);
}

bool Input::IsRelease(int keyCode)
{
    return
        (m_currentKey[keyCode] == 0) &&
        (m_prevKey[keyCode] != 0);
}

bool Input::IsPadPress(int button)
{
    return m_currentPad.Buttons[button] != 0;
}

bool Input::IsPadTrigger(int button)
{
    return
        (m_currentPad.Buttons[button] != 0) &&
        (m_prevPad.Buttons[button] == 0);
}

bool Input::IsPadRelease(int button)
{
    return
        (m_currentPad.Buttons[button] == 0) &&
        (m_prevPad.Buttons[button] != 0);
}

bool Input::IsLTPress()
{
    if (!m_xInputAvailable)
        return false;

    return m_currentXPad.LeftTrigger > 0;
}

bool Input::IsLTTrigger()
{
    if (!m_xInputAvailable)
        return false;

    return
        (m_currentXPad.LeftTrigger > 0) &&
        (m_prevXPad.LeftTrigger == 0);
}

float Input::GetLT()
{
    if (!m_xInputAvailable)
        return 0.0f;

    return m_currentXPad.LeftTrigger / 255.0f;
}

bool Input::IsRTPress()
{
    if (!m_xInputAvailable)
        return false;

    return m_currentXPad.RightTrigger > 0;
}

bool Input::IsRTTrigger()
{
    if (!m_xInputAvailable)
        return false;

    return
        (m_currentXPad.RightTrigger > 0) &&
        (m_prevXPad.RightTrigger == 0);
}

float Input::GetRT()
{
    if (!m_xInputAvailable)
        return 0.0f;

    return m_currentXPad.RightTrigger / 255.0f;
}

float Input::GetLeftStickX()
{
    return m_currentPad.X / 1000.0f;
}

float Input::GetLeftStickY()
{
    return m_currentPad.Y / 1000.0f;
}

float Input::GetRightStickX()
{
    return m_currentPad.Rx / 1000.0f;
}

float Input::GetRightStickY()
{
    return m_currentPad.Ry / 1000.0f;
}

bool Input::IsL3Press()
{
    return IsPadPress(8);
}

bool Input::IsL3Trigger()
{
    return IsPadTrigger(8);
}

bool Input::IsR3Press()
{
    return IsPadPress(9);
}

bool Input::IsR3Trigger()
{
    return IsPadTrigger(9);
}

bool Input::IsDPadUp()
{
    return m_currentPad.POV[0] == 0;
}

bool Input::IsDPadDown()
{
    return
        (m_currentPad.POV[0] == 18000) ||
        (m_currentPad.POV[0] == 13500) ||
        (m_currentPad.POV[0] == 22500);
}

bool Input::IsDPadLeft()
{
    return
        (m_currentPad.POV[0] == 27000) ||
        (m_currentPad.POV[0] == 22500) ||
        (m_currentPad.POV[0] == 31500);
}

bool Input::IsDPadRight()
{
    return
        (m_currentPad.POV[0] == 9000) ||
        (m_currentPad.POV[0] == 4500) ||
        (m_currentPad.POV[0] == 13500);
}