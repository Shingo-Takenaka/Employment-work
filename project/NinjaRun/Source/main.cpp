#include "DxLib.h"

#include "Camera/Camera.h"
#include "Field/Field.h"
#include "Ninja/Ninja.h"
#include "Input/Input.h"

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

    // カメラの描画範囲
    SetCameraNearFar(0.1f, 5000.0f);

    //ライト
    //SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));

    // 環境光（全体を少し明るくする）
    //SetGlobalAmbientLight(GetColorF(0.4f, 0.4f, 0.4f, 0.0f));

    //視野角の調整
    SetCameraDotAspect(1.0f);
    SetupCamera_Perspective(DX_PI_F / 4.0f); // 45度くらい

    Camera camera;
    Field field;
    Ninja ninja;

    VECTOR playerPos = VGet(0.0f, 0.0f, 0.0f); // 変数の宣言と初期化

    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        Input::Update();

        ClearDrawScreen();

        ninja.Update();

        camera.Update(ninja.GetPosition());

        field.Draw();

        ninja.Draw();

        camera.Draw();

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}