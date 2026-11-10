#pragma once

#include <memory>
#include "SceneBase.h"
#include "../Ninja/Ninja.h"

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
    // 次のシーンを生成
    std::unique_ptr<SceneBase> CreateScene(SceneType sceneType);

    // シーンの切り替えを実行
    void ApplySceneChange();

    // 現在のシーン
    std::unique_ptr<SceneBase> m_currentScene;

    // 遷移先のシーン
    SceneType m_nextSceneType;

    // シーン切り替え要求
    bool m_hasSceneChangeRequest;

    //ステージ間で引き継ぐNinjaの情報
    PlayerData m_playerData;
};