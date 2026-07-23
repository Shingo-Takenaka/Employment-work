#include "SceneManager.h"

SceneManager::SceneManager()
{
    m_scene = SceneType::PLAY;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
    switch (m_scene)
    {
    case SceneType::PLAY:
        m_playScene.Update();
        break;
    }
}

void SceneManager::Draw()
{
    switch (m_scene)
    {
    case SceneType::PLAY:
        m_playScene.Draw();
        break;
    }
}

void SceneManager::ChangeScene(SceneType scene)
{
    m_scene = scene;
}