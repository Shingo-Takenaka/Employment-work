#include "DxLib.h"

#include "Camera/Camera.h"
#include "Field/Field.h"

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

    //ライト
    SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));

    // 環境光（全体を少し明るくする）
    SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 0.0f));

    Camera camera;

    Field field;

    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        ClearDrawScreen();

        camera.Update();

        field.Draw();

        camera.Draw();

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}