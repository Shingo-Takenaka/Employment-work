#include "TitleScene.h"
#include "../SceneManager.h"
#include "DxLib.h"

TitleScene::TitleScene(SceneManager& sceneManager)
    : m_sceneManager(sceneManager),
    m_wasKeyPressed(false)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
    m_wasKeyPressed = false;
}

void TitleScene::Load()
{
}

void TitleScene::Start()
{
}

void TitleScene::Update()
{
    char keyState[256] = {};

    GetHitKeyStateAll(keyState);

    bool isKeyPressed = false;

    for (int i = 0; i < 256; i++)
    {
        if (keyState[i] != 0)
        {
            isKeyPressed = true;
            break;
        }
    }

    if (isKeyPressed && !m_wasKeyPressed)
    {
        m_sceneManager.ChangeScene(SceneType::Play);
    }

    m_wasKeyPressed = isKeyPressed;
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
