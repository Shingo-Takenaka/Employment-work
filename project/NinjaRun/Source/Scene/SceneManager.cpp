#include "SceneManager.h"
#include "TitleScene/TitleScene.h"
#include "PlayScene.h"
#include <utility>

SceneManager::SceneManager()
    : m_nextSceneType(SceneType::Title),
    m_hasSceneChangeRequest(false)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
    ChangeScene(SceneType::Title);
    ApplySceneChange();
}

void SceneManager::Update()
{
    if (m_currentScene)
    {
        m_currentScene->Update();
    }

    if (m_hasSceneChangeRequest)
    {
        ApplySceneChange();
    }
}

void SceneManager::Draw()
{
    if (m_currentScene)
    {
        m_currentScene->Draw();
    }
}

void SceneManager::ChangeScene(SceneType nextScene)
{
    m_nextSceneType = nextScene;
    m_hasSceneChangeRequest = true;
}

std::unique_ptr<SceneBase> SceneManager::CreateScene(SceneType sceneType)
{
    switch (sceneType)
    {
    case SceneType::Title:
        return std::make_unique<TitleScene>(*this);

    case SceneType::Play:
        return std::make_unique<PlayScene>();
    }

    return nullptr;
}

void SceneManager::ApplySceneChange()
{
    if (!m_hasSceneChangeRequest)
    {
        return;
    }

    // 次のシーンを生成
    std::unique_ptr<SceneBase> nextScene =
        CreateScene(m_nextSceneType);

    // 生成できなかった場合は切り替えない
    if (!nextScene)
    {
        m_hasSceneChangeRequest = false;
        return;
    }

    // 次のシーンの初期化と読み込み
    nextScene->Init();
    nextScene->Load();

    // 現在のシーンを破棄して切り替える
    m_currentScene = std::move(nextScene);

    // 切り替え要求を解除
    m_hasSceneChangeRequest = false;

    // 新しいシーンを開始
    m_currentScene->Start();
}

