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
    // 基本設定
    m_pos = VGet(0.0f, 0.0f, 0.0f);
    m_moveSpeed = 0.5f;
    m_size = 10.0f;

    // アニメーション読み込み
    m_ninjaAnimation.LoadAnimations();

    // 向き
    m_isReverseX = false;

    // 最後に移動した方向
    m_lastMoveDirectionX = 1.0f;
    m_lastMoveDirectionZ = 0.0f;

    m_lastHorizontalInput = 0;
    m_lastVerticalInput = 0;
    m_prevA = false;
    m_prevD = false;
    m_prevW = false;
    m_prevS = false;

    m_prevDash = false;

    // ダッシュ方向
    m_dashDirectionX = 1.0f;
    m_dashDirectionZ = 0.0f;

    // ダッシュ
    m_isDash = false;
    m_dashTimer = 0.0f;
    m_dashSpeed = DASH_ACCEL_SPEED;
    m_dashAccelTimer = 0.0f;
    m_isDashCooldown = false;
    m_dashCooldownTimer = 0.0f;

    // ジャンプ
    m_isJump = false;
    m_groundY = m_pos.y;
    m_jumpSpeed = 0.0f;
    m_gravity = 0.05f;

    // 壁キック
    m_canWallKick = false;
    m_wallKickDirection = VGet(0.0f, 0.0f, 0.0f);
    m_wallKickGroundY = m_pos.y;
    m_wallKickInputTimer = 0.0f;

    // 攻撃
    m_isSlash = false;
    m_isShoot = false;
    m_isShootStart = false;

    // ガード
    m_isGuard = false;

    // ノックバック
    m_isKnockback = false;
    m_knockbackDirection = VGet(0.0f, 0.0f, 0.0f);
    m_knockbackStrength = 0.0f;
    m_knockbackTimer = 0.0f;

    // 手裏剣の方向
    m_lastShootDirection = 0;
}

Ninja::~Ninja()
{
}

void Ninja::Update()
{
    // 手裏剣を投げたタイミングを毎フレームリセット
    m_isShootStart = false;

    // ダッシュのクールタイムを更新
    if (m_isDashCooldown)
    {
        m_dashCooldownTimer -= 1.0f / 60.0f;

        if (m_dashCooldownTimer <= 0.0f)
        {
            m_dashCooldownTimer = 0.0f;
            m_isDashCooldown = false;
        }
    }

    // 壁キック後の時間を減らす
    if (m_wallKickInputTimer > 0.0f)
    {
        m_wallKickInputTimer -= 1.0f / 60.0f;

        if (m_wallKickInputTimer < 0.0f)
        {
            m_wallKickInputTimer = 0.0f;
        }
    }

    // ノックバック中なら通常操作を行わない
    if (m_isKnockback)
    {
        m_isDash = false;
        m_dashTimer = 0.0f;

        m_pos.x += m_knockbackDirection.x * m_knockbackStrength;
        m_pos.z += m_knockbackDirection.z * m_knockbackStrength;

        m_knockbackTimer -= 1.0f / 60.0f;

        if (m_knockbackTimer <= 0.0f)
        {
            m_knockbackTimer = 0.0f;
            m_isKnockback = false;
            m_knockbackStrength = 0.0f;
        }

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

    // 入力情報を取得
    const NinjaInputState& input = m_input.GetState();

    // ガード終了
    if (m_isGuard && !input.guard)
    {
        m_isGuard = false;
    }

    // ガード開始
    if (input.guard &&
        !m_isJump &&
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

    // Jキーでダッシュ斬りを開始
    if (input.dashCut)
    {
        float directionX = m_lastMoveDirectionX;
        float directionZ = m_lastMoveDirectionZ;

        // 移動入力があれば、その方向を優先する
        if (input.isMove)
        {
            directionX = input.moveX;
            directionZ = input.moveZ;
        }

        m_ninjaAttack.StartDashCut(directionX, directionZ);
    }

    // ダッシュ斬り中
    if (m_ninjaAttack.IsDashCutActive())
    {
        m_ninjaAttack.UpdateDashCut();

        if (m_ninjaAttack.IsDashCutActive())
        {
            // X方向とZ方向の両方に移動する
            m_pos.x +=
                m_ninjaAttack.GetDashCutDirectionX() * 1.5f;

            m_pos.z +=
                m_ninjaAttack.GetDashCutDirectionZ() * 1.5f;

            // 左右反転
            if (m_ninjaAttack.GetDashCutDirectionX() < 0.0f)
            {
                m_isReverseX = true;
            }
            else if (m_ninjaAttack.GetDashCutDirectionX() > 0.0f)
            {
                m_isReverseX = false;
            }

            m_prevDash = input.dash;

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
    }

    // ダッシュ処理
    UpdateDash();

    m_prevDash = input.dash;

    // ダッシュ中のジャンプ入力
    if (m_isDash &&
        input.jump &&
        !m_isJump &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        m_isJump = true;
        m_groundY = m_pos.y;
        m_jumpSpeed = 1.0f;
        m_canWallKick = false;
    }

    // ダッシュ中は通常移動を行わない
    if (m_isDash)
    {
        // ジャンプ処理は継続する
        if (m_isJump)
        {
            m_pos.y += m_jumpSpeed;
            m_jumpSpeed -= m_gravity;

            if (m_pos.y <= m_groundY)
            {
                m_pos.y = m_groundY;
                m_isJump = false;
                m_jumpSpeed = 0.0f;
                m_canWallKick = false;
            }
        }

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

    // ガード・近接攻撃・手裏剣中はX/Z移動しない
    if (m_isGuard ||
        m_isSlash ||
        m_isShoot)
    {
        // ジャンプ処理だけは継続
        if (m_isJump)
        {
            m_pos.y += m_jumpSpeed;
            m_jumpSpeed -= m_gravity;

            if (m_pos.y <= m_groundY)
            {
                m_pos.y = m_groundY;
                m_isJump = false;
                m_jumpSpeed = 0.0f;
                m_canWallKick = false;
            }
        }

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

    // 壁キック直後の移動
    if (m_wallKickInputTimer > 0.0f)
    {
        if (input.isMove)
        {
            m_pos.x += input.moveX * m_moveSpeed;
            m_pos.z += input.moveZ * m_moveSpeed;

            // 最後に移動した方向を記憶
            RememberMoveDirection(input.moveX, input.moveZ);

            // 左右反転
            if (input.moveX < 0.0f)
            {
                m_isReverseX = true;
            }
            else if (input.moveX > 0.0f)
            {
                m_isReverseX = false;
            }
        }
        else
        {
            // WASDを押していない場合は壁キック方向へ飛び続ける
            m_pos.x += m_wallKickDirection.x * WALL_KICK_SPEED;
            m_pos.z += m_wallKickDirection.z * WALL_KICK_SPEED;
        }
    }
    else if (m_dashTimer <= 0.0f)
    {
        if (input.isMove)
        {
            m_pos.x += input.moveX * m_moveSpeed;
            m_pos.z += input.moveZ * m_moveSpeed;

            // 最後に移動した方向を記憶
            RememberMoveDirection(input.moveX, input.moveZ);
        }
    }

    // ジャンプ
    if (input.jump &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        if (!m_isJump)
        {
            // 通常ジャンプ
            m_isJump = true;
            m_groundY = m_pos.y;
            m_jumpSpeed = 1.0f;
            m_canWallKick = false;
        }
        else if (m_canWallKick)
        {
            // 空中で壁に触れている場合は壁キック
            WallKick();
        }
    }

    // ジャンプ処理
    if (m_isJump)
    {
        m_pos.y += m_jumpSpeed;
        m_jumpSpeed -= m_gravity;

        if (m_pos.y <= m_groundY)
        {
            m_pos.y = m_groundY;
            m_isJump = false;
            m_jumpSpeed = 0.0f;
            m_canWallKick = false;
        }
    }

    // 左右反転
    if (!m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        if (input.moveX < 0.0f)
        {
            m_isReverseX = true;
        }
        else if (input.moveX > 0.0f)
        {
            m_isReverseX = false;
        }
    }

    // アニメーション更新
    m_ninjaAnimation.Update(
        input.isMove,
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

bool Ninja::IsKnockback() const
{
    return m_isKnockback;
}

bool Ninja::IsDash() const
{
    return m_isDash;
}

bool Ninja::IsSlash() const
{
    return m_isSlash;
}

bool Ninja::IsReverseX() const
{
    return m_isReverseX;
}

bool Ninja::IsShoot() const
{
    return m_isShoot;
}

bool Ninja::IsShootStart() const
{
    return m_isShootStart;
}

int Ninja::GetShootDirection() const
{
    return m_lastShootDirection;
}

bool Ninja::IsJump() const
{
    return m_isJump;
}

bool Ninja::CanWallKick() const
{
    return m_canWallKick;
}

bool Ninja::IsDashCut() const
{
    return m_ninjaAttack.IsDashCutActive();
}

void Ninja::EnableWallKick(VECTOR direction)
{
    // 空中にいるときだけ壁キック可能にする
    if (!m_isJump)
    {
        return;
    }

    m_canWallKick = true;
    m_wallKickDirection = direction;
}

void Ninja::WallKick()
{
    // 壁キック可能でなければ何もしない
    if (!m_canWallKick)
    {
        return;
    }

    // 上方向へ強く飛ぶ
    m_jumpSpeed = 1.5f;

    // m_groundYは変更しない
    // 壁キック前のジャンプ開始地点を着地点として使う
    m_isJump = true;

    // 壁キック権を消費
    m_canWallKick = false;

    // 壁キック後は一定時間、キック方向への移動を行う
    m_wallKickInputTimer = WALL_KICK_MOVE_TIME;
}