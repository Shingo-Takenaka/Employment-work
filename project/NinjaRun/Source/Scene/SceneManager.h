#pragma once

#include "PlayScene.h"

// シーンの種類
enum class SceneType
{
    PLAY,
};

class SceneManager
{
public:

    SceneManager();
    ~SceneManager();

    void Update();
    void Draw();

    // シーン変更
    void ChangeScene(SceneType scene);

private:

    SceneType m_scene;

    PlayScene m_playScene;
};