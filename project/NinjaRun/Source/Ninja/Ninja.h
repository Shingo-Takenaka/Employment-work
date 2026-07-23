#pragma once

#include "DxLib.h"
#include "../Animation/Animation.h"

enum class NinjaAnim
{
    WAIT,       // 待機
    WALK,       // 歩き
    JUMP,       // ジャンプ
    SLASH,      // 袈裟斬り
    GUARD,      // ガード

    MAX
};

// 入力情報
struct NinjaInputState
{
    // 移動方向
    float moveX;
    float moveZ;

    // 移動しているか
    bool isMove;

    // 各入力
    bool jump;
    bool slash;
    bool guard;
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
    void UpdateInput();

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

    // 入力情報
    NinjaInputState m_input;

    // 状態

    // ジャンプ中
    bool m_isJump;

    // ジャンプ開始時の高さ
    float m_groundY;

    // ジャンプ速度
    float m_jumpSpeed;

    // 重力
    float m_gravity;

    // 袈裟斬り中
    bool m_isSlash;

    // ガード中
    bool m_isGuard;

    // アニメーション
    SpriteAnimation m_animation[(int)NinjaAnim::MAX];

    NinjaAnim m_currentAnim;
};