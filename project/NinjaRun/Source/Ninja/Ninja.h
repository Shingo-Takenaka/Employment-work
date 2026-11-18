#pragma once

#include "DxLib.h"
#include "NinjaAnimation.h"
#include "NinjaInput.h"
#include "NinjaAttack.h"

struct PlayerData
{
    // HP
    int MaxHP = 10;
    int NowHP = 10;

    // 攻撃力
    int SlashPower = 1;
    int ShurikenPower = 1;
    int DashCutPower = 1;
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

    // アクセサ
    bool IsKnockback() const { return m_isKnockback; }
    bool IsDash() const { return m_isDash; }
    bool IsSlash() const { return m_isSlash; }
    bool IsReverseX() const { return m_isReverseX; }
    bool IsShoot() const { return m_isShoot; }
    bool IsShootStart() const { return m_isShootStart; }
    bool IsJump() const { return m_isJump; }
    bool IsGrounded() const { return m_isGrounded; }
    bool CanWallKick() const { return m_canWallKick; }
    bool IsDashCut() const { return m_ninjaAttack.IsDashCutActive(); }
    bool IsMove() const { return m_input.IsMove(); }
    int GetSlashPower() const { return m_data.SlashPower; }
    int GetShurikenPower() const { return m_data.ShurikenPower; }
    int GetDashCutPower() const { return m_data.DashCutPower; }

    // 地面との接触
    void ResolveGroundCollision(float groundY);
    void SetAirborne();

    // 壁との接触
    void SetWallContact(VECTOR direction);

    // 攻撃方向
    int GetShootDirection() const { return m_lastShootDirection; }
    float GetDashCutDirectionX() const;
    float GetDashCutDirectionZ() const;

    // ノックバック
    void ApplyKnockback(
        VECTOR direction,
        float strength,
        float duration);

    // ステータス
    PlayerData GetData() const;
    void SetData(const PlayerData& data);

    int GetAttackPower(AttackType attackType) const;

private:

    PlayerData m_data;

    // 入力関連
    NinjaInput m_input;

    int m_lastHorizontalInput;
    int m_lastVerticalInput;

    bool m_prevA;
    bool m_prevD;
    bool m_prevW;
    bool m_prevS;
    bool m_prevDash;

    // 座標・基本情報
    VECTOR m_pos;
    float m_size;

    // 移動関連
    float m_moveSpeed;

    float m_lastMoveDirectionX;
    float m_lastMoveDirectionZ;

    bool m_isReverseX;

    void RememberMoveDirection(float moveX, float moveZ);

    // ダッシュ関連
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

    // ジャンプ・落下関連
    bool m_isJump;
    bool m_isGrounded;

    float m_jumpSpeed;
    float m_gravity;

    void UpdateVerticalMovement();

    // 壁キック関連
    static const float WALL_KICK_MOVE_TIME;
    static const float WALL_KICK_SPEED;

    bool m_canWallKick;
    bool m_isWallKicking;

    VECTOR m_wallKickDirection;

    float m_wallKickInputTimer;

    float m_wallKickVelocityX;
    float m_wallKickVelocityZ;

    bool WallkickUpdate(const NinjaInputState& input);

    // 攻撃関連
    bool m_isSlash;
    bool m_isGuard;
    bool m_isShoot;
    bool m_isShootStart;

    int m_lastShootDirection;

    NinjaAttack m_ninjaAttack;

    // アニメーション関連
    NinjaAnimation m_ninjaAnimation;

    // ノックバック関連
    bool m_isKnockback;

    VECTOR m_knockbackDirection;

    float m_knockbackStrength;
    float m_knockbackTimer;
};
