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

    // 敵からのノックバック
    void ApplyKnockback(VECTOR direction, float strength, float duration);

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

    int GetShootDirection() const { return m_lastShootDirection; }


    void SetWallContact(VECTOR direction);

    float GetDashCutDirectionX() const;
    float GetDashCutDirectionZ() const;

    PlayerData GetData() const;
    void SetData(const PlayerData& data);

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

    //=====移動関連=====
    // 最後に移動した方向
    float m_lastMoveDirectionX;
    float m_lastMoveDirectionZ;

    float m_moveSpeed;

    // 左右反転
    bool m_isReverseX;

    //=====ダッシュ関連=====
    // ダッシュ方向
    float m_dashDirectionX;
    float m_dashDirectionZ;

    bool m_isDash;

    // WASDを押し始めてからの経過時間
    float m_dashTimer;

    float m_dashSpeed;

    // Lのみで加速している時間
    float m_dashAccelTimer;

    bool m_isDashCooldown;
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

    // =====壁キック関連=====
    // 壁キックできる状態か
    bool m_canWallKick;

    // 壁っキックの強制移動中か
    bool m_isWallKicking;
    
    // 壁から離れる方向
    VECTOR m_wallKickDirection;
    
    // 壁キック後に戻る地面のY座標
    float m_wallKickGroundY;
    
    // 壁キック後の入力無効時間
    float m_wallKickInputTimer;

    // キック方向の速度
    float m_wallKickVelocityX;
    float m_wallKickVelocityZ;

    bool WallkickUpdate(const NinjaInputState& input);

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