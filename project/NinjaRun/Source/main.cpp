#include "DxLib.h"

#include "Input/Input.h"
#include "Scene/SceneManager.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

int WINAPI WinMain(_In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, _In_ int nShowCmd)

{
    SetOutApplicationLogValidFlag(FALSE);

    // ウィンドウモードON
    ChangeWindowMode(TRUE);

    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

    if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
    {
        return -1;			// エラーが起きたら直ちに終了
    }

    SetDrawScreen(DX_SCREEN_BACK);

    SetUseLighting(FALSE);

    // カメラの描画範囲
    SetCameraNearFar(0.1f, 5000.0f);

    //ライト
    //SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));

    // 環境光（全体を少し明るくする）
    //SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 0.0f));

    //視野角の調整
    SetCameraDotAspect(1.0f);
    SetupCamera_Perspective(DX_PI_F / 4.0f); // 45度くらい

    SceneManager sceneManager;

    // 60FPS固定用
    const LONGLONG frameTime = 1000000 / 60; // 1フレーム = 約16666マイクロ秒
    LONGLONG nextFrameTime = GetNowHiPerformanceCount() + frameTime;

    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        Input::Update();

        sceneManager.Update();

        ClearDrawScreen();

        sceneManager.Draw();

        ScreenFlip();

        // 次のフレームまで待つ
        LONGLONG nowTime = GetNowHiPerformanceCount();

        if (nowTime < nextFrameTime)
        {
            // マイクロ秒 → ミリ秒
            int waitTime = (int)((nextFrameTime - nowTime) / 1000);

            if (waitTime > 0)
            {
                WaitTimer(waitTime);
            }
        }

        // 次のフレームの予定時刻
        nextFrameTime += frameTime;

        // 処理落ちなどで大幅に遅れた場合の補正
        nowTime = GetNowHiPerformanceCount();

        if (nextFrameTime < nowTime)
        {
            nextFrameTime = nowTime + frameTime;
        }

    }

    DxLib_End();

    return 0;
}