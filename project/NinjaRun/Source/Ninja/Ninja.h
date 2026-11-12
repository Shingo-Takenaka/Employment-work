#pragma once

#include "DxLib.h"
#include "NinjaAnimation.h"
#include "NinjaInput.h"
#include "NinjaAttack.h"

struct PlayerData
{
    //ここにはプレイ中変更されるであろう数値だけを書く
    int MaxHP = 10;
    int NowHP = 10;
    int AttackPower = 1;
};

class Ninja
{
public:
    Ninja();
    ~Ninja();

    void Init();
    void Start();
    void Load();
    void Update();
    void Draw();

    VECTOR GetPosition() const;
    void SetPosition(VECTOR pos);

    //アクセサ
    bool IsKnockback() const { return m_isKnockback; }
    bool IsDash() const{ return m_isDash; }
    bool IsSlash() const{ return m_isSlash; }
    bool IsReverseX() const{ return m_isReverseX; }
    bool IsShoot() const{ return m_isShoot; }
    bool IsShootStart() const{ return m_isShootStart; }
    bool IsJump() const{ return m_isJump; }
    bool CanWallKick() const{ return m_canWallKick; }
    bool IsDashCut() const{ return m_ninjaAttack.IsDashCutActive(); }
    bool IsMove() const { return m_input.IsMove(); }

    //壁の接触
    void SetWallContact(VECTOR direction);

    //攻撃方向
    int GetShootDirection() const { return m_lastShootDirection; }
    float GetDashCutDirectionX() const;
    float GetDashCutDirectionZ() const;

    //ノックバック
    void ApplyKnockback(VECTOR direction, float strength, float duration);

    //ステータス
    PlayerData GetData() const;
    void SetData(const PlayerData& data);

private:
    //入力関連
    NinjaInput m_input;

    int m_lastHorizontalInput;
    int m_lastVerticalInput;

    bool m_prevA;
    bool m_prevD;
    bool m_prevW;
    bool m_prevS;
    bool m_prevDash;

    //座標・基本情報
    VECTOR m_pos;
    float m_size;

    //移動関連
    float m_moveSpeed;

    float m_lastMoveDirectionX;
    float m_lastMoveDirectionZ;

    bool m_isReverseX;

    void RememberMoveDirection(float moveX, float moveZ);

    //ダッシュ関連
    static const float DASH_ACCEL_SPEED;
    static const float DASH_L_ONLY_TIME;
    static const float DASH_STATE_TIME;
    static const float DASH_SPEED;
    static const float DASH_COOLDOWN;

    float m_dashDirectionX;
    float m_dashDirectionZ;

    bool m_isDash;

    float m_dashTimer;
    float m_dashSpeed;
    float m_dashAccelTimer;

    bool m_isDashCooldown;
    float m_dashCooldownTimer;

    void UpdateDash();

    //ジャンプ関連
    bool m_isJump;

    float m_groundY;
    float m_jumpSpeed;
    float m_gravity;

    //壁キック関連
    static const float WALL_KICK_MOVE_TIME;
    static const float WALL_KICK_SPEED;

    bool m_canWallKick;
    bool m_isWallKicking;

    VECTOR m_wallKickDirection;

    float m_wallKickGroundY;
    float m_wallKickInputTimer;

    float m_wallKickVelocityX;
    float m_wallKickVelocityZ;

    bool WallkickUpdate(const NinjaInputState& input);

    //攻撃関連
    bool m_isSlash;
    bool m_isGuard;
    bool m_isShoot;
    bool m_isShootStart;

    int m_lastShootDirection;

    NinjaAttack m_ninjaAttack;

    //アニメーション関連
    NinjaAnimation m_ninjaAnimation;

    //ノックバック関連
    bool m_isKnockback;

    VECTOR m_knockbackDirection;

    float m_knockbackStrength;
    float m_knockbackTimer;
};