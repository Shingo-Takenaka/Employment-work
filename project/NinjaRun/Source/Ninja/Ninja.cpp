#include "Ninja.h"
#include <cmath>

// ダッシュ開始直後の速度
const float Ninja::DASH_ACCEL_SPEED = 1.2f;
// Lのみを押したときに移動する時間
const float Ninja::DASH_L_ONLY_TIME = 0.1f;
// ダッシュ状態になるまでの時間
const float Ninja::DASH_STATE_TIME = 0.4f;
// ダッシュ中の速度
const float Ninja::DASH_SPEED = 0.8f;
// ダッシュクールタイム
const float Ninja::DASH_COOLDOWN = 0.5f;

// 壁キック後の移動時間
const float Ninja::WALL_KICK_MOVE_TIME = 0.2f;
// 壁キック後の移動速度
const float Ninja::WALL_KICK_SPEED = 1.0f;

Ninja::Ninja()
{
    // 入力関連
    m_lastHorizontalInput = 0;
    m_lastVerticalInput = 0;

    m_prevA = false;
    m_prevD = false;
    m_prevW = false;
    m_prevS = false;
    m_prevDash = false;

    // 座標・基本情報
    m_pos = VGet(0.0f, 0.0f, 0.0f);
    m_size = 10.0f;

    // 移動関連
    m_moveSpeed = 0.5f;

    m_lastMoveDirectionX = 1.0f;
    m_lastMoveDirectionZ = 0.0f;

    m_isReverseX = false;

    // ダッシュ関連
    m_dashDirectionX = 1.0f;
    m_dashDirectionZ = 0.0f;

    m_isDash = false;

    m_dashTimer = 0.0f;
    m_dashSpeed = DASH_ACCEL_SPEED;
    m_dashAccelTimer = 0.0f;

    m_isDashCooldown = false;
    m_dashCooldownTimer = 0.0f;

    // ジャンプ・落下関連
    m_isJump = false;
    m_isGrounded = false;

    m_jumpSpeed = 0.0f;
    m_gravity = 0.05f;

    // 壁キック関連
    m_canWallKick = false;
    m_isWallKicking = false;

    m_wallKickDirection = VGet(0.0f, 0.0f, 0.0f);

    m_wallKickInputTimer = 0.0f;

    m_wallKickVelocityX = 0.0f;
    m_wallKickVelocityZ = 0.0f;

    // 攻撃関連
    m_isSlash = false;
    m_isGuard = false;
    m_isShoot = false;
    m_isShootStart = false;

    m_lastShootDirection = 0;

    // アニメーション関連
    m_ninjaAnimation.SetNinja(this);

    // ノックバック関連
    m_isKnockback = false;

    m_knockbackDirection = VGet(0.0f, 0.0f, 0.0f);

    m_knockbackStrength = 0.0f;
    m_knockbackTimer = 0.0f;
}

Ninja::~Ninja()
{
}

void Ninja::Init()
{
    /*
    
    m_Ninjapos = VGet(0.0, 0.0, 0.0);
    
    */
}

void Ninja::Start()
{
}

void Ninja::Load()
{
    // アニメーション読み込み
    m_ninjaAnimation.LoadAnimations();
}


void Ninja::Update()
{
    // 手裏剣の発射タイミングをリセット
    m_isShootStart = false;

    const float deltaTime = 1.0f / 60.0f;

    // ダッシュのクールタイム
    if (m_isDashCooldown)
    {
        m_dashCooldownTimer -= deltaTime;

        if (m_dashCooldownTimer <= 0.0f)
        {
            m_dashCooldownTimer = 0.0f;
            m_isDashCooldown = false;
        }
    }

    // ノックバック
    if (m_isKnockback)
    {
        m_isDash = false;
        m_dashTimer = 0.0f;

        m_pos.x +=
            m_knockbackDirection.x * m_knockbackStrength;

        m_pos.z +=
            m_knockbackDirection.z * m_knockbackStrength;

        m_knockbackTimer -= deltaTime;

        if (m_knockbackTimer <= 0.0f)
        {
            m_knockbackTimer = 0.0f;
            m_isKnockback = false;
            m_knockbackStrength = 0.0f;
        }

        // ノックバック中も重力を適用
        UpdateVerticalMovement();

        m_ninjaAnimation.Update(
            false,
            m_isJump,
            m_jumpSpeed,
            m_isSlash,
            m_isGuard,
            m_isShoot,
            m_isDash,
            m_ninjaAttack.IsDashCutActive());

        return;
    }

    // 入力更新
    m_input.Update();

    const NinjaInputState& input = m_input.GetState();

    // ガード終了
    if (m_isGuard && !input.guard)
    {
        m_isGuard = false;
    }

    // ガード開始
    if (input.guard &&
        m_isGrounded &&
        !m_isSlash &&
        !m_isShoot &&
        !m_isGuard)
    {
        m_isGuard = true;
        m_ninjaAnimation.Play(NinjaAnim::GUARD);
    }

    // 近接攻撃
    if (input.slash &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        m_isSlash = true;
        m_ninjaAnimation.Play(NinjaAnim::SLASH);
    }

    // 遠距離攻撃
    if (input.shoot &&
        !m_isShoot &&
        !m_isGuard &&
        !m_isSlash)
    {
        m_isShoot = true;
        m_ninjaAnimation.Play(NinjaAnim::SHOOT);
        m_isShootStart = true;
    }

    // ダッシュ斬り開始
    if (input.dashCut)
    {
        float directionX = m_lastMoveDirectionX;
        float directionZ = m_lastMoveDirectionZ;

        if (input.isMove)
        {
            directionX = input.moveX;
            directionZ = input.moveZ;
        }

        m_ninjaAttack.StartDashCut(
            directionX,
            directionZ);
    }

    bool isMoving = false;
    bool isDashCutMoving = false;

    // ダッシュ斬り中
    if (m_ninjaAttack.IsDashCutActive())
    {
        m_ninjaAttack.UpdateDashCut();

        if (m_ninjaAttack.IsDashCutActive())
        {
            m_pos.x +=
                m_ninjaAttack.GetDashCutDirectionX() * 1.5f;

            m_pos.z +=
                m_ninjaAttack.GetDashCutDirectionZ() * 1.5f;

            float directionX =
                m_ninjaAttack.GetDashCutDirectionX();

            if (directionX < 0.0f)
            {
                m_isReverseX = true;
            }
            else if (directionX > 0.0f)
            {
                m_isReverseX = false;
            }

            isDashCutMoving = true;
        }
    }

    if (!isDashCutMoving)
    {
        // ダッシュ処理
        UpdateDash();

        // ダッシュ中もジャンプを開始できる
        if (input.jump &&
            m_isGrounded &&
            !m_isSlash &&
            !m_isGuard &&
            !m_isShoot)
        {
            m_isGrounded = false;
            m_isJump = true;
            m_jumpSpeed = 1.0f;
            m_canWallKick = false;
        }

        if (!m_isDash &&
            !m_isGuard &&
            !m_isSlash &&
            !m_isShoot)
        {
            // 壁キック
            bool isWallKickMoving = WallkickUpdate(input);

            if (!isWallKickMoving)
            {
                // 左右反転
                if (input.moveX < 0.0f)
                {
                    m_isReverseX = true;
                }
                else if (input.moveX > 0.0f)
                {
                    m_isReverseX = false;
                }

                // 通常移動
                if (input.isMove)
                {
                    m_pos.x += input.moveX * m_moveSpeed;
                    m_pos.z += input.moveZ * m_moveSpeed;

                    isMoving = true;
                }
            }
        }
    }

    m_prevDash = input.dash;

    // すべての移動状態で重力を適用
    UpdateVerticalMovement();

    // アニメーション更新
    m_ninjaAnimation.Update(
        isMoving,
        m_isJump,
        m_jumpSpeed,
        m_isSlash,
        m_isGuard,
        m_isShoot,
        m_isDash,
        m_ninjaAttack.IsDashCutActive());
}


void Ninja::RememberMoveDirection(float moveX, float moveZ)
{
    float length = sqrtf(moveX * moveX + moveZ * moveZ);

    if (length <= 0.001f)
    {
        return;
    }

    // 移動方向を正規化して保存
    m_lastMoveDirectionX = moveX / length;
    m_lastMoveDirectionZ = moveZ / length;

    // 手裏剣の方向は従来どおり4方向で保持する
    if (fabsf(m_lastMoveDirectionX) >= fabsf(m_lastMoveDirectionZ))
    {
        m_lastShootDirection =
            m_lastMoveDirectionX >= 0.0f ? 0 : 1;
    }
    else
    {
        m_lastShootDirection =
            m_lastMoveDirectionZ >= 0.0f ? 2 : 3;
    }

    if (moveX < 0.0f)
    {
        m_isReverseX = true;
    }
    else if (moveX > 0.0f)
    {
        m_isReverseX = false;
    }
}

void Ninja::UpdateDash()
{
    const NinjaInputState& input = m_input.GetState();
    const float deltaTime = 1.0f / 60.0f;

    // 移動方向を正規化して保存する
    auto setDashDirection = [this](float moveX, float moveZ)
        {
            float length = sqrtf(moveX * moveX + moveZ * moveZ);

            if (length > 0.001f)
            {
                m_dashDirectionX = moveX / length;
                m_dashDirectionZ = moveZ / length;
            }
        };

    // 保存した方向へ指定速度で移動する
    auto moveDash = [this](float speed)
        {
            m_pos.x += m_dashDirectionX * speed;
            m_pos.z += m_dashDirectionZ * speed;

            if (m_dashDirectionX < 0.0f)
            {
                m_isReverseX = true;
            }
            else if (m_dashDirectionX > 0.0f)
            {
                m_isReverseX = false;
            }
        };

    // ダッシュ中
    if (m_isDash)
    {
        // ガード・攻撃中は移動しないが、ダッシュ状態は維持する
        if (m_isGuard || m_isSlash || m_isShoot)
        {
            return;
        }

        // ダッシュ中にLをもう一度押したら再加速
        if (input.dash && !m_prevDash && !m_isDashCooldown)
        {
            m_dashTimer = 0.0f;
            m_dashAccelTimer = 0.0f;
            m_dashSpeed = DASH_ACCEL_SPEED;

            m_isDashCooldown = true;
            m_dashCooldownTimer = DASH_COOLDOWN;

            if (input.isMove)
            {
                setDashDirection(input.moveX, input.moveZ);
            }
        }

        // 再加速中
        if (m_dashSpeed == DASH_ACCEL_SPEED &&
            m_dashTimer < DASH_STATE_TIME)
        {
            // WASDがなくなったらダッシュ終了
            if (!input.isMove)
            {
                m_isDash = false;
                m_dashTimer = 0.0f;
                m_dashAccelTimer = 0.0f;
                m_dashSpeed = 0.0f;
                return;
            }

            // 再加速中の方向変更
            setDashDirection(input.moveX, input.moveZ);
            RememberMoveDirection(input.moveX, input.moveZ);

            m_dashTimer += deltaTime;
            m_dashSpeed = DASH_ACCEL_SPEED;
            moveDash(m_dashSpeed);

            // 0.4秒経過したら通常のダッシュ速度へ戻す
            if (m_dashTimer >= DASH_STATE_TIME)
            {
                m_dashTimer = DASH_STATE_TIME;
                m_dashSpeed = DASH_SPEED;
            }

            return;
        }

        // WASDがなくなったらダッシュ終了
        if (!input.isMove)
        {
            m_isDash = false;
            m_dashTimer = 0.0f;
            m_dashAccelTimer = 0.0f;
            m_dashSpeed = 0.0f;
            return;
        }

        // ダッシュ中も入力方向へ移動する
        setDashDirection(input.moveX, input.moveZ);
        RememberMoveDirection(input.moveX, input.moveZ);

        m_dashSpeed = DASH_SPEED;
        moveDash(m_dashSpeed);

        return;
    }

    // ダッシュ開始前の加速処理
    if (m_dashTimer > 0.0f)
    {
        // ガード・攻撃中は移動せず、タイマーも進めない
        if (m_isGuard || m_isSlash || m_isShoot)
        {
            return;
        }

        // WASDがない場合は、最後に移動した方向へ短時間だけ加速
        if (!input.isMove)
        {
            m_dashAccelTimer += deltaTime;
            m_dashSpeed = DASH_ACCEL_SPEED;
            moveDash(m_dashSpeed);

            if (m_dashAccelTimer >= DASH_L_ONLY_TIME)
            {
                m_dashSpeed = 0.0f;
                m_dashTimer = 0.0f;
                m_dashAccelTimer = 0.0f;
            }

            return;
        }

        // WASDがある場合は入力方向へ加速する
        setDashDirection(input.moveX, input.moveZ);
        RememberMoveDirection(input.moveX, input.moveZ);

        m_dashTimer += deltaTime;
        m_dashSpeed = DASH_ACCEL_SPEED;
        moveDash(m_dashSpeed);

        if (m_dashTimer >= DASH_STATE_TIME)
        {
            m_dashTimer = DASH_STATE_TIME;
            m_isDash = true;
            m_dashSpeed = DASH_SPEED;
        }

        return;
    }

    // Lを押した瞬間だけダッシュを開始
    if (input.dash && !m_prevDash && !m_isDashCooldown)
    {
        m_dashTimer = deltaTime;
        m_dashAccelTimer = 0.0f;

        // Lを押した時点でクールタイム開始
        m_isDashCooldown = true;
        m_dashCooldownTimer = DASH_COOLDOWN;

        if (input.isMove)
        {
            setDashDirection(input.moveX, input.moveZ);
            RememberMoveDirection(input.moveX, input.moveZ);
        }
        else
        {
            // WASDを押していない場合は最後の移動方向を使う
            setDashDirection(
                m_lastMoveDirectionX,
                m_lastMoveDirectionZ);
        }

        m_dashSpeed = DASH_ACCEL_SPEED;
        moveDash(m_dashSpeed);
    }
}


bool Ninja::WallkickUpdate(const NinjaInputState& input)
{
    const float deltaTime = 1.0f / 60.0f;

    // 壁に接触している空中状態でジャンプ
    if (!m_isWallKicking &&
        m_canWallKick &&
        m_isJump &&
        input.jump)
    {
        m_isWallKicking = true;
        m_canWallKick = false;

        // 上向きの初速
        m_jumpSpeed = 1.5f;

        // 壁から離れる方向
        m_wallKickVelocityX =
            m_wallKickDirection.x * WALL_KICK_SPEED;

        m_wallKickVelocityZ =
            m_wallKickDirection.z * WALL_KICK_SPEED;

        m_wallKickInputTimer = WALL_KICK_MOVE_TIME;

        if (m_wallKickVelocityX < 0.0f)
        {
            m_isReverseX = true;
        }
        else if (m_wallKickVelocityX > 0.0f)
        {
            m_isReverseX = false;
        }
    }

    if (!m_isWallKicking)
    {
        return false;
    }

    // 壁キック中の水平移動
    m_pos.x += m_wallKickVelocityX;
    m_pos.z += m_wallKickVelocityZ;

    m_wallKickInputTimer -= deltaTime;

    if (m_wallKickInputTimer <= 0.0f)
    {
        m_wallKickInputTimer = 0.0f;
        m_isWallKicking = false;

        m_wallKickVelocityX = 0.0f;
        m_wallKickVelocityZ = 0.0f;
    }

    return true;
}


void Ninja::SetWallContact(VECTOR direction)
{
    // 空中にいるときだけ壁キック可能にする
    if (!m_isJump || m_isWallKicking)
    {
        return;
    }

    m_canWallKick = true;
    m_wallKickDirection = direction;
}

void Ninja::Draw()
{
    m_ninjaAnimation.Draw(
        m_pos,
        m_size,
        m_isReverseX);
}

VECTOR Ninja::GetPosition() const
{
    return m_pos;
}

void Ninja::SetPosition(VECTOR pos)
{
    m_pos = pos;
}

void Ninja::ApplyKnockback(
    VECTOR direction,
    float strength,
    float duration)
{
    m_isKnockback = true;

    m_knockbackDirection = direction;
    m_knockbackStrength = strength;
    m_knockbackTimer = duration;

    // ダッシュを解除
    m_isDash = false;
    m_dashTimer = 0.0f;
}

float Ninja::GetDashCutDirectionX() const
{
    return m_ninjaAttack.GetDashCutDirectionX();
}

float Ninja::GetDashCutDirectionZ() const
{
    return m_ninjaAttack.GetDashCutDirectionZ();
}

void Ninja::UpdateVerticalMovement()
{
    // 地面に接している場合
    if (m_isGrounded)
    {
        m_jumpSpeed = 0.0f;
        return;
    }

    // 上下方向の移動
    m_pos.y += m_jumpSpeed;

    // 重力を適用
    m_jumpSpeed -= m_gravity;

    // 空中状態
    m_isJump = true;
}

void Ninja::ResolveGroundCollision(float groundY)
{
    // 上昇中は着地させない
    if (m_jumpSpeed > 0.0f)
    {
        return;
    }

    // 地面の高さへ補正
    m_pos.y = groundY;

    // 接地状態に変更
    m_isGrounded = true;
    m_isJump = false;

    m_jumpSpeed = 0.0f;

    // 壁キック状態を解除
    m_canWallKick = false;
    m_isWallKicking = false;

    m_wallKickVelocityX = 0.0f;
    m_wallKickVelocityZ = 0.0f;
    m_wallKickInputTimer = 0.0f;
}

void Ninja::SetAirborne()
{
    m_isGrounded = false;
    m_isJump = true;
}