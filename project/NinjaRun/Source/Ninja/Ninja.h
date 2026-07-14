#pragma once

#include "DxLib.h"
#include "../Animation/Animation.h"

enum class NinjaAnim
{
    WAIT,       // 待機
    WALK,       // 歩き
    JUMP,       // ジャンプ
    SLASH,      // 袈裟斬り

    MAX
};

// アニメーション情報
struct SpriteAnimation
{
    // アニメーションに使える最大フレーム数
    static const int MAX_FRAME = 16;

    // 分割した画像
    int graph[MAX_FRAME];

    // フレーム数
    int frameNum;

    // アニメーション
    Animation anim;
};

class Ninja
{
public:

    Ninja();
    ~Ninja();

    void Update();
    void Draw();

    VECTOR GetPosition() const;

private:

    // 入力処理
    void UpdateInput(
        bool& isMove,
        bool& jump,
        bool& slash,
        float& moveX,
        float& moveZ);

    // アニメーション読み込み
    void LoadAnimations();

    // アニメーション1種類読み込み
    bool LoadAnimation(
        SpriteAnimation& animation,
        const char* fileName,
        int frameNum,
        int width,
        int height,
        int interval);

    // アニメーション更新
    void UpdateAnimation(bool isMove);

    // アニメーション描画
    void DrawAnimation();

private:

    // プレイヤー情報
    VECTOR m_pos;

    float m_size;

    float m_moveSpeed;

    // 左右反転
    bool m_isReverseX;

    // ジャンプ
    bool m_isJump;

    // ジャンプ開始時の高さ
    float m_groundY;

    // ジャンプ速度
    float m_jumpSpeed;

    // 重力
    float m_gravity;

    // 攻撃
    // 袈裟斬り中か
    bool m_isSlash;

    // アニメーション
    SpriteAnimation m_animation[(int)NinjaAnim::MAX];

    NinjaAnim m_currentAnim;
};