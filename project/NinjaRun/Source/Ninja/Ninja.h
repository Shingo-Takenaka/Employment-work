#pragma once

#include "DxLib.h"
#include "../Animation/Animation.h"
#include "NinjaInput.h"

enum class NinjaAnim
{
    WAIT,       // 待機
    WALK,       // 歩き
    JUMP,       // 上昇
    FALLING,    // 落下
    SLASH,      // 袈裟斬り
    GUARD,      // ガード
    SHOOT,      // 手裏剣

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
    void SetPosition(VECTOR pos);

    // 敵からのノックバック
    void ApplyKnockback(
        VECTOR direction, float strength, float duration);

    // ノックバック中か
    bool IsKnockback() const;

    bool IsSlash() const;
    bool IsShoot() const;
    bool IsReverseX() const;

    // 手裏剣を投げたタイミング
    bool IsShootStart() const;

    int GetShootDirection() const;

    // ダッシュ中か
    bool IsDash() const;

    // ジャンプ中か
    bool IsJump() const;

    // 壁キックできる状態か
    bool CanWallKick() const;

    // 壁に触れたので壁キック可能にする
    void EnableWallKick(VECTOR direction);

    // 壁キック
    void WallKick();

private:

    // 入力処理
    void UpdateInput();

    // ダッシュ処理
    void UpdateDash();

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


    // 入力関連
    int m_lastHorizontalInput;
    int m_lastVerticalInput;

    bool m_prevA;
    bool m_prevD;
    bool m_prevW;
    bool m_prevS;

    // Lキーの前フレーム状態
    bool m_prevDash;

    // 入力情報
    NinjaInputState m_input;


    // -----移動関連-----

    // 最後に移動した方向
    // 0 = +X
    // 1 = -X
    // 2 = +Z
    // 3 = -Z
    int m_lastMoveDirection;

    // 移動速度
    float m_moveSpeed;

    // 左右反転
    bool m_isReverseX;


    // -----ダッシュ関連-----

    // ダッシュ方向
    int m_dashDirection;

    // ダッシュ中か
    bool m_isDash;

    // WASDを押し始めてからの経過時間
    float m_dashTimer;

    // ダッシュ速度
    float m_dashSpeed;

    // Lのみで1.5の速度で移動している時間
    float m_dashAccelTimer;


    // -----ジャンプ関連-----

    // ジャンプ中
    bool m_isJump;

    // ジャンプ開始時の高さ
    float m_groundY;

    // ジャンプ速度
    float m_jumpSpeed;

    // 重力
    float m_gravity;


    // -----壁キック関連-----

    // 壁キックできる状態か
    bool m_canWallKick;

    // 壁から離れる方向
    VECTOR m_wallKickDirection;

    // 壁キック後に戻る地面のY座標
    float m_wallKickGroundY;

    // 壁キック後の入力無効時間
    float m_wallKickInputTimer;


    // -----攻撃関連-----

    // 袈裟斬り中
    bool m_isSlash;

    // ガード中
    bool m_isGuard;

    // 手裏剣中
    bool m_isShoot;

    // 手裏剣を投げたタイミング
    bool m_isShootStart;

    // 手裏剣を投げる方向
    int m_lastShootDirection;


    // プレイヤー情報

    VECTOR m_pos;

    float m_size;


    // アニメーション関連

    SpriteAnimation m_animation[(int)NinjaAnim::MAX];

    NinjaAnim m_currentAnim;


    // ノックバック関連

    // ノックバック中
    bool m_isKnockback;

    // ノックバック方向
    VECTOR m_knockbackDirection;

    // ノックバック強度
    float m_knockbackStrength;

    // ノックバック残り時間
    float m_knockbackTimer;
};