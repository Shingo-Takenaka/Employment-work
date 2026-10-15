
#include "NinjaInput.h"

#include "../Input/Input.h"
#include <math.h>

NinjaInput::NinjaInput()
{
    m_state.moveX = 0.0f;
    m_state.moveZ = 0.0f;
    m_state.jump = false;
    m_state.dash = false;
    m_state.guard = false;
    m_state.isMove = false;
    m_state.slash = false;
    m_state.dashCut = false;
    m_state.shoot = false;

    m_prevA = false;
    m_prevD = false;
    m_prevW = false;
    m_prevS = false;

    m_lastMoveDirection = MoveDirection::RIGHT;
}

void NinjaInput::Update()
{
    m_state.moveX = 0.0f;
    m_state.moveZ = 0.0f;
    m_state.isMove = false;

    m_state.jump = false;
    m_state.dash = false;
    m_state.guard = false;

    m_state.slash = false;
    m_state.dashCut = false;
    m_state.shoot = false;

    // キーボード入力
    bool pressA = Input::IsPress(KEY_INPUT_A);
    bool pressD = Input::IsPress(KEY_INPUT_D);
    bool pressW = Input::IsPress(KEY_INPUT_W);
    bool pressS = Input::IsPress(KEY_INPUT_S);

    // 最後に押された移動方向を記録
    if (pressA && !m_prevA)
    {
        m_lastMoveDirection = MoveDirection::LEFT;
    }

    if (pressD && !m_prevD)
    {
        m_lastMoveDirection = MoveDirection::RIGHT;
    }

    if (pressW && !m_prevW)
    {
        m_lastMoveDirection = MoveDirection::BACK;
    }

    if (pressS && !m_prevS)
    {
        m_lastMoveDirection = MoveDirection::FRONT;
    }

    // A・Dの入力
    if (pressA && pressD)
    {
        if (m_lastMoveDirection == MoveDirection::LEFT)
        {
            m_state.moveX = -1.0f;
        }
        else if (m_lastMoveDirection == MoveDirection::RIGHT)
        {
            m_state.moveX = 1.0f;
        }
    }
    else if (pressA)
    {
        m_state.moveX = -1.0f;
    }
    else if (pressD)
    {
        m_state.moveX = 1.0f;
    }

    // W・Sの入力
    if (pressW && pressS)
    {
        if (m_lastMoveDirection == MoveDirection::BACK)
        {
            m_state.moveZ = 1.0f;
        }
        else if (m_lastMoveDirection == MoveDirection::FRONT)
        {
            m_state.moveZ = -1.0f;
        }
    }
    else if (pressW)
    {
        m_state.moveZ = 1.0f;
    }
    else if (pressS)
    {
        m_state.moveZ = -1.0f;
    }

    // 次のフレーム用にキー状態を保存
    m_prevA = pressA;
    m_prevD = pressD;
    m_prevW = pressW;
    m_prevS = pressS;

    // ジャンプ
    if (Input::IsTrigger(KEY_INPUT_SPACE))
    {
        m_state.jump = true;
    }

    // 近距離攻撃
    if (Input::IsTrigger(KEY_INPUT_M))
    {
        m_state.slash = true;
    }

    // ガード
    if (Input::IsPress(KEY_INPUT_K))
    {
        m_state.guard = true;
    }

    // 手裏剣
    if (Input::IsTrigger(KEY_INPUT_N))
    {
        m_state.shoot = true;
    }

    // ダッシュ
    if (Input::IsPress(KEY_INPUT_L))
    {
        m_state.dash = true;
    }

    // ダッシュ斬り
    if (Input::IsTrigger(KEY_INPUT_J))
    {
        m_state.dashCut = true;
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

    // Aボタン：ジャンプ
    if (Input::IsPadTrigger(0))
    {
        m_state.jump = true;
    }

    // Bボタン：ダッシュ
    if (Input::IsPadPress(1))
    {
        m_state.dash = true;
    }

    // Xボタン：近距離攻撃
    if (Input::IsPadTrigger(2))
    {
        m_state.slash = true;
    }

    // Yボタン：手裏剣
    if (Input::IsPadTrigger(3))
    {
        m_state.shoot = true;
    }

    // RBボタン：ガード
    if (joyState.Buttons[5])
    {
        m_state.guard = true;
    }

    // 左スティック
    // スティック入力がある場合はキーボードより優先
    if (stickX != 0.0f || stickY != 0.0f)
    {
        m_state.moveX = stickX;
        m_state.moveZ = -stickY;
    }

    // 斜め移動補正
    float length = sqrtf(
        m_state.moveX * m_state.moveX +
        m_state.moveZ * m_state.moveZ);

    if (length > 1.0f)
    {
        m_state.moveX /= length;
        m_state.moveZ /= length;
    }

    // 移動判定
    if (m_state.moveX != 0.0f ||
        m_state.moveZ != 0.0f)
    {
        m_state.isMove = true;
    }
}

const NinjaInputState& NinjaInput::GetState() const
{
    return m_state;
}

MoveDirection NinjaInput::GetLastMoveDirection() const
{
    return m_lastMoveDirection;
}
