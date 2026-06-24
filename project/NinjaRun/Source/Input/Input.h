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

private:
    static char m_currentKey[256]; // 今回のフレームのキー状態
    static char m_prevKey[256];    // 1つ前のフレームのキー状態
};