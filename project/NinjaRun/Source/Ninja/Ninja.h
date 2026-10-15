#pragma once

#include "DxLib.h"
#include "NinjaAnimation.h"
#include "NinjaInput.h"
#include "NinjaAttack.h"

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
    void ApplyKnockback(VECTOR direction, float strength, float duration);

    // ノックバック中か
    bool IsKnockback() const;

    bool IsSlash() const;
    bool IsShoot() const;
    bool IsReverseX() const;

    // 手裏剣を投げたタイミング
    bool IsShootStart() const;

    int GetShootDirection() const;

    // ダッシュ中
    bool IsDash() const;

    // ジャンプ中
    bool IsJump() const;

    // ダッシュ斬り中
    bool IsDashCut() const;

    // 壁キックできる状態か
    bool CanWallKick() const;

    // 壁に触れたので壁キック可能にする
    void EnableWallKick(VECTOR direction);

    // 壁キック
    void WallKick();

private:
    // ダッシュ関連の定数
    static const float DASH_ACCEL_SPEED;
    static const float DASH_L_ONLY_TIME;
    static const float DASH_STATE_TIME;
    static const float DASH_SPEED;

    // 壁キック関連の定数
    static const float WALL_KICK_MOVE_TIME;
    static const float WALL_KICK_SPEED;

    static const float DASH_COOLDOWN;

    // ダッシュ処理
    void UpdateDash();

    // 入力関連
    int m_lastHorizontalInput;
    int m_lastVerticalInput;

    bool m_prevA;
    bool m_prevD;
    bool m_prevW;
    bool m_prevS;

    // Lキーの前フレーム状態
    bool m_prevDash;

    // 入力管理
    NinjaInput m_input;

    // 移動関連

    // 最後に移動した方向
    float m_lastMoveDirectionX;
    float m_lastMoveDirectionZ;

    // 移動速度
    float m_moveSpeed;

    // 左右反転
    bool m_isReverseX;

    // ダッシュ関連

    // ダッシュ方向
    float m_dashDirectionX;
    float m_dashDirectionZ;

    // ダッシュ中か
    bool m_isDash;

    // WASDを押し始めてからの経過時間
    float m_dashTimer;

    // ダッシュ速度
    float m_dashSpeed;

    // Lのみで加速している時間
    float m_dashAccelTimer;

    // ダッシュのクールタイム中か
    bool m_isDashCooldown;

    // クールタイムの残り時間
    float m_dashCooldownTimer;

    // 移動方向を記憶する関数
    void RememberMoveDirection(float moveX, float moveZ);

    // ジャンプ関連

    // ジャンプ中
    bool m_isJump;

    // ジャンプ開始時の高さ
    float m_groundY;

    // ジャンプ速度
    float m_jumpSpeed;

    // 重力
    float m_gravity;

    // 壁キック関連

    // 壁キックできる状態か
    bool m_canWallKick;

    // 壁から離れる方向
    VECTOR m_wallKickDirection;

    // 壁キック後に戻る地面のY座標
    float m_wallKickGroundY;

    // 壁キック後の入力無効時間
    float m_wallKickInputTimer;

    // 攻撃関連

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

    // ダッシュ斬り管理
    NinjaAttack m_ninjaAttack;

    // プレイヤー情報
    VECTOR m_pos;

    float m_size;

    NinjaAnimation m_ninjaAnimation;

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