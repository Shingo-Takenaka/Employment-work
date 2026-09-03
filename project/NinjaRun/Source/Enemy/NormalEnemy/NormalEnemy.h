#pragma once

#include "DxLib.h"
#include "../../Animation/Animation.h"

class Ninja;

// アニメーション種類
enum class NormalEnemyAnim
{
    WAIT,
    SHOOT,

    MAX
};

// アニメーション情報
struct EnemySpriteAnimation
{
    static const int MAX_FRAME = 16;

    int graph[MAX_FRAME];

    int frameNum;

    Animation anim;
};

class NormalEnemy
{
public:

    NormalEnemy();
    ~NormalEnemy();

    void Update(const Ninja& ninja);
    void Draw();

    // 座標取得
    VECTOR GetPosition() const;

    // 座標設定
    void SetPosition(VECTOR pos);

private:

    // アニメーション読み込み
    void LoadAnimations();

    // アニメーション1種類読み込み
    bool LoadAnimation(
        EnemySpriteAnimation& animation,
        const char* fileName,
        int frameNum,
        int width,
        int height,
        int interval);

private:

    // 基本情報
    VECTOR m_pos;

    float m_size;

    // アニメーション
    EnemySpriteAnimation
        m_animation[(int)NormalEnemyAnim::MAX];

    NormalEnemyAnim m_currentAnim;
};