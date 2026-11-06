#pragma once

#include <memory>
#include "SceneBase.h"

enum class SceneType
{
    Title,
    Play
    //Result
};

/*
今後こんなかんじでステージ分けをしたい
enum class StageType
{
    A1,
    A2,
    A3,

    B1,
    B2,
    B3
};
*/

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Init();
    void Update();
    void Draw();

    void ChangeScene(SceneType nextScene);

private:
    std::unique_ptr<SceneBase> m_currentScene;
    std::unique_ptr<SceneBase> m_nextScene;
};