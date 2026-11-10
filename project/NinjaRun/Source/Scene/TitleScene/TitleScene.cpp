#include "TitleScene.h"
#include "DxLib.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
}

void TitleScene::Load()
{
}

void TitleScene::Start()
{
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
    DrawString(
        650,
        430,
        "Please Push Any Key",
        GetColor(255, 255, 255)
    );
}
