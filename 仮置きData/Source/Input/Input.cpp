#include "Input.h"

// 静的メンバ変数の実体定義
char Input::m_currentKey[256] = { 0 };
char Input::m_prevKey[256] = { 0 };

void Input::Update()
{
    // 前回の状態を保存
    for (int i = 0; i < 256; i++)
    {
        m_prevKey[i] = m_currentKey[i];
    }

    // 最新の状態を取得
    GetHitKeyStateAll(m_currentKey);
}

bool Input::IsPress(int keyCode)
{
    // 0でなければ押されている
    return m_currentKey[keyCode] != 0;
}

bool Input::IsTrigger(int keyCode)
{
    // 今回押されていて、前回押されていなければ「今押された瞬間」
    return (m_currentKey[keyCode] != 0) && (m_prevKey[keyCode] == 0);
}

bool Input::IsRelease(int keyCode)
{
    // 今回押されていなくて、前回押されていたら「今離された瞬間」
    return (m_currentKey[keyCode] == 0) && (m_prevKey[keyCode] != 0);
}