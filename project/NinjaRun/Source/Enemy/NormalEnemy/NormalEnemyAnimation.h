#pragma once

#include "DxLib.h"

#include "../../Animation/Animation.h"

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

class NormalEnemyAnimation
{
public:

    NormalEnemyAnimation();
    ~NormalEnemyAnimation();

    // アニメーション初期化
    void LoadAnimations();

    // アニメーション更新
    void Update();

    // 現在のアニメーションを設定
    void SetAnimation(NormalEnemyAnim anim);

    // 現在のアニメーションを取得
    NormalEnemyAnim GetCurrentAnimation() const;

    // 現在のフレームを取得
    int GetFrame() const;

    // アニメーションリセット
    void Reset();

    // 描画
    void DrawAnimation(
        VECTOR pos,
        float size,
        bool isReverseX);

private:

    // アニメーション1種類読み込み
    bool LoadAnimation(
        EnemySpriteAnimation& animation,
        const char* fileName,
        int frameNum,
        int width,
        int height,
        int interval);

private:

    EnemySpriteAnimation
        m_animation[(int)NormalEnemyAnim::MAX];

    NormalEnemyAnim m_currentAnim;
};