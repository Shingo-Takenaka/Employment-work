
#pragma once

#include "DxLib.h"

#include "../../Animation/Animation.h"

// アニメーション種類
enum class WalkEnemyAnim
{
    WAIT,
    WALK,

    MAX
};

// アニメーション情報
struct WalkEnemySpriteAnimation
{
    static const int MAX_FRAME = 16;

    int graph[MAX_FRAME];

    int frameNum;

    Animation anim;
};

class WalkEnemyAnimation
{
public:
    WalkEnemyAnimation();
    ~WalkEnemyAnimation();

    // アニメーション読み込み
    void LoadAnimations();

    // アニメーション更新
    void Update();

    // アニメーション変更
    void SetAnimation(WalkEnemyAnim anim);

    // 現在のアニメーション取得
    WalkEnemyAnim GetCurrentAnimation() const;

    // 現在のフレーム取得
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
        WalkEnemySpriteAnimation& animation,
        const char* fileName,
        int frameNum,
        int width,
        int height,
        int interval);

private:
    WalkEnemySpriteAnimation m_animation[
        (int)WalkEnemyAnim::MAX];

    WalkEnemyAnim m_currentAnim;
};