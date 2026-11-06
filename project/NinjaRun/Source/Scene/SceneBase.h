#pragma once

class SceneBase
{
public:
    virtual ~SceneBase() = default;

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
};