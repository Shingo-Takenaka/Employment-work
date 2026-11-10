#pragma once

#include "../SceneBase.h"

class SceneManager;

class TitleScene : public SceneBase
{
public:
    explicit TitleScene(SceneManager& sceneManager);
    ~TitleScene() override;

    void Init() override;
    void Load() override;
    void Start() override;
    void Update() override;
    void Draw() override;

private:
    SceneManager& m_sceneManager;

    // 前フレームにキーが押されていたか
    bool m_wasKeyPressed;
};
