#include "Ninja.h"

namespace
{
    // Lを押した直後の速度
    const float DASH_ACCEL_SPEED = 0.8f;

    // Lのみを押したときに1.0の速度で移動する時間
    const float DASH_L_ONLY_TIME = 0.2f;

    // L + WASDを押し続けてダッシュ状態になるまでの時間
    const float DASH_STATE_TIME = 0.4f;

    // ダッシュ中の速度
    const float DASH_SPEED = 0.8f;

    // 壁キック後、キック方向へ移動する時間
    const float WALL_KICK_MOVE_TIME = 0.2f;

    // 壁キック中の移動速度
    const float WALL_KICK_SPEED = 1.0f;
}

Ninja::Ninja()
{
    // 基本設定
    m_pos = VGet(0.0f, 0.0f, 0.0f);

    m_moveSpeed = 0.5f;

    m_size = 10.0f;

    // アニメーション読み込み
    LoadAnimations();

    m_currentAnim = NinjaAnim::WAIT;

    // 向き
    m_isReverseX = false;

    // 最後に移動した方向
    // 最初は右方向
    m_lastMoveDirection = 0;

    m_lastHorizontalInput = 0;
    m_lastVerticalInput = 0;
    m_prevA = false;
    m_prevD = false;
    m_prevW = false;
    m_prevS = false;

    m_prevDash = false;

    // ダッシュ方向
    m_dashDirection = 0;

    // ダッシュ
    m_isDash = false;
    m_dashTimer = 0.0f;
    m_dashSpeed = DASH_ACCEL_SPEED;
    m_dashAccelTimer = 0.0f;

    // ジャンプ
    m_isJump = false;

    m_groundY = m_pos.y;

    m_jumpSpeed = 0.0f;

    m_gravity = 0.05f;

    // 壁キック
    m_canWallKick = false;

    m_wallKickDirection = VGet(0.0f, 0.0f, 0.0f);

    m_wallKickInputTimer = 0.0f;

    // 攻撃
    m_isSlash = false;
    m_isShoot = false;

    // ガード
    m_isGuard = false;

    // ノックバック
    m_isKnockback = false;

    m_knockbackDirection = VGet(0.0f, 0.0f, 0.0f);

    m_knockbackStrength = 0.0f;

    m_knockbackTimer = 0.0f;

    // 手裏剣発射タイミング
    m_isShootStart = false;

    // 最初は右方向
    m_lastShootDirection = 0;
}

Ninja::~Ninja()
{
}

void Ninja::Update()
{
    // 手裏剣を投げたタイミングを毎フレームリセット
    m_isShootStart = false;

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
        // ダッシュを解除
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

        UpdateAnimation(false);

        return;
    }

    // 入力更新
    UpdateInput();

    // ガード終了
    // Kを離したらガードだけ終了し、
    // ダッシュ状態はそのまま維持する
    if (m_isGuard && !m_input.guard)
    {
        m_isGuard = false;
    }

    // ガード開始
    if (m_input.guard &&
        !m_isJump &&
        !m_isSlash &&
        !m_isShoot &&
        !m_isGuard)
    {
        m_isGuard = true;

        m_currentAnim = NinjaAnim::GUARD;

        m_animation[(int)NinjaAnim::GUARD].anim.Reset();
    }

    // 近接攻撃
    if (m_input.slash &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        m_isSlash = true;

        m_currentAnim = NinjaAnim::SLASH;

        m_animation[(int)NinjaAnim::SLASH].anim.Reset();
    }

    // 遠距離攻撃
    if (m_input.shoot &&
        !m_isShoot &&
        !m_isGuard &&
        !m_isSlash)
    {
        // 手裏剣攻撃開始
        m_isShoot = true;

        // SHOOTアニメーションを最初から再生
        m_currentAnim = NinjaAnim::SHOOT;

        m_animation[(int)NinjaAnim::SHOOT].anim.Reset();

        // このフレームで手裏剣を1個発射
        m_isShootStart = true;
    }

    // ダッシュ処理
    // K・M・Nの状態を先に設定してから呼ぶことで、
    // アクション開始フレームからダッシュ移動を止める
    UpdateDash();

    m_prevDash = m_input.dash;

    // K・M・Nのアクション中はX/Z移動しない
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

        // アニメーション更新
        UpdateAnimation(false);

        return;
    }

    // 壁キック直後の移動
    if (m_wallKickInputTimer > 0.0f)
    {
        // WASDが入力されている場合
        // 通常のWASD入力を優先する
        if (m_input.isMove)
        {
            m_pos.x += m_input.moveX * m_moveSpeed;

            m_pos.z += m_input.moveZ * m_moveSpeed;

            // 最後に移動した方向を記憶
            if (m_input.moveZ > 0.0f)
            {
                // W
                m_lastMoveDirection = 2;
                m_lastShootDirection = 2;
            }
            else if (m_input.moveZ < 0.0f)
            {
                // S
                m_lastMoveDirection = 3;
                m_lastShootDirection = 3;
            }
            else if (m_input.moveX > 0.0f)
            {
                // D
                m_lastMoveDirection = 0;
                m_lastShootDirection = 0;
            }
            else if (m_input.moveX < 0.0f)
            {
                // A
                m_lastMoveDirection = 1;
                m_lastShootDirection = 1;
            }

            // 左右反転
            if (m_input.moveX < 0.0f)
            {
                m_isReverseX = true;
            }
            else if (m_input.moveX > 0.0f)
            {
                m_isReverseX = false;
            }
        }
        else
        {
            // WASDを押していない場合は
            // 壁キック方向へ飛び続ける
            m_pos.x +=
                m_wallKickDirection.x *
                WALL_KICK_SPEED;

            m_pos.z +=
                m_wallKickDirection.z *
                WALL_KICK_SPEED;
        }
    }
    // ダッシュ中はUpdateDash()で移動しているため
    // 通常移動を行わない
    else if (!m_isDash)
    {
        if (m_input.isMove)
        {
            m_pos.x +=
                m_input.moveX *
                m_moveSpeed;

            m_pos.z +=
                m_input.moveZ *
                m_moveSpeed;

            // 最後に移動した方向を記憶
            if (m_input.moveZ > 0.0f)
            {
                // W
                m_lastMoveDirection = 2;
                m_lastShootDirection = 2;
            }
            else if (m_input.moveZ < 0.0f)
            {
                // S
                m_lastMoveDirection = 3;
                m_lastShootDirection = 3;
            }
            else if (m_input.moveX > 0.0f)
            {
                // D
                m_lastMoveDirection = 0;
                m_lastShootDirection = 0;
            }
            else if (m_input.moveX < 0.0f)
            {
                // A
                m_lastMoveDirection = 1;
                m_lastShootDirection = 1;
            }
        }
    }

    // ジャンプ
    if (m_input.jump &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        // 通常ジャンプ
        if (!m_isJump)
        {
            m_isJump = true;

            m_groundY = m_pos.y;

            m_jumpSpeed = 1.0f;

            // 通常ジャンプを開始したら
            // 壁キック権を消費する
            m_canWallKick = false;
        }
        // 空中で壁に触れている場合は壁キック
        else if (m_canWallKick)
        {
            WallKick();
        }
    }

    // ジャンプ処理
    if (m_isJump)
    {
        m_pos.y += m_jumpSpeed;

        m_jumpSpeed -= m_gravity;

        // 常に通常ジャンプ開始時の地面Yを基準にする
        // 壁キックではm_groundYを変更しない
        if (m_pos.y <= m_groundY)
        {
            m_pos.y = m_groundY;

            m_isJump = false;

            m_jumpSpeed = 0.0f;

            // 着地したので壁キック権を消費
            m_canWallKick = false;
        }
    }

    // 左右反転
    if (!m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        if (m_input.moveX < 0.0f)
        {
            m_isReverseX = true;
        }
        else if (m_input.moveX > 0.0f)
        {
            m_isReverseX = false;
        }
    }

    // アニメーション更新
    UpdateAnimation(m_input.isMove);
}

void Ninja::UpdateDash()
{
    const float deltaTime = 1.0f / 60.0f;

    // ダッシュ中
    if (m_isDash)
    {
        // K・M・Nのアクション中は移動しない
        // ただしダッシュ状態は維持する
        if (m_isGuard || m_isSlash || m_isShoot)
        {
            return;
        }

        // ダッシュ中にLをもう一度押したら再加速
        if (m_input.dash && !m_prevDash)
        {
            m_dashTimer = 0.0f;
            m_dashAccelTimer = 0.0f;
            m_dashSpeed = DASH_ACCEL_SPEED;
        }

        // 再加速中
        if (m_dashSpeed == DASH_ACCEL_SPEED && m_dashTimer < DASH_STATE_TIME)
        {
            // WASDがなくなったらダッシュ終了
            if (!m_input.isMove)
            {
                m_isDash = false;
                m_dashTimer = 0.0f;
                m_dashAccelTimer = 0.0f;
                m_dashSpeed = 0.0f;

                return;
            }

            m_dashTimer += deltaTime;

            // 再加速中の方向変更
            if (m_input.moveZ > 0.0f)
            {
                m_dashDirection = 2;
            }
            else if (m_input.moveZ < 0.0f)
            {
                m_dashDirection = 3;
            }
            else if (m_input.moveX > 0.0f)
            {
                m_dashDirection = 0;
            }
            else if (m_input.moveX < 0.0f)
            {
                m_dashDirection = 1;
            }

            m_dashSpeed = DASH_ACCEL_SPEED;

            switch (m_dashDirection)
            {
            case 0:
                m_pos.x += m_dashSpeed;
                m_isReverseX = false;
                break;

            case 1:
                m_pos.x -= m_dashSpeed;
                m_isReverseX = true;
                break;

            case 2:
                m_pos.z += m_dashSpeed;
                break;

            case 3:
                m_pos.z -= m_dashSpeed;
                break;
            }

            // 0.4秒経過したら通常のダッシュ速度へ戻す
            if (m_dashTimer >= DASH_STATE_TIME)
            {
                m_dashTimer = DASH_STATE_TIME;
                m_dashSpeed = DASH_SPEED;
            }

            return;
        }

        // WASDがなくなったらダッシュ終了
        if (!m_input.isMove)
        {
            m_isDash = false;
            m_dashTimer = 0.0f;
            m_dashAccelTimer = 0.0f;
            m_dashSpeed = 0.0f;

            return;
        }

        // ダッシュ中の方向変更
        if (m_input.moveZ > 0.0f)
        {
            m_dashDirection = 2;
        }
        else if (m_input.moveZ < 0.0f)
        {
            m_dashDirection = 3;
        }
        else if (m_input.moveX > 0.0f)
        {
            m_dashDirection = 0;
        }
        else if (m_input.moveX < 0.0f)
        {
            m_dashDirection = 1;
        }

        m_dashSpeed = DASH_SPEED;

        switch (m_dashDirection)
        {
        case 0:
            m_pos.x += m_dashSpeed;
            m_isReverseX = false;
            break;

        case 1:
            m_pos.x -= m_dashSpeed;
            m_isReverseX = true;
            break;

        case 2:
            m_pos.z += m_dashSpeed;
            break;

        case 3:
            m_pos.z -= m_dashSpeed;
            break;
        }

        return;
    }

    // ダッシュ開始前の処理中
    if (m_dashTimer > 0.0f)
    {
        // K・M・Nのアクション中は移動しない
        // ダッシュ開始前なのでタイマーも進めない
        if (m_isGuard || m_isSlash || m_isShoot)
        {
            return;
        }

        // WASDがない場合
        if (!m_input.isMove)
        {
            m_dashAccelTimer += deltaTime;
            m_dashSpeed = DASH_ACCEL_SPEED;

            switch (m_dashDirection)
            {
            case 0:
                m_pos.x += m_dashSpeed;
                m_isReverseX = false;
                break;

            case 1:
                m_pos.x -= m_dashSpeed;
                m_isReverseX = true;
                break;

            case 2:
                m_pos.z += m_dashSpeed;
                break;

            case 3:
                m_pos.z -= m_dashSpeed;
                break;
            }

            if (m_dashAccelTimer >= DASH_L_ONLY_TIME)
            {
                m_dashSpeed = 0.0f;
                m_dashTimer = 0.0f;
                m_dashAccelTimer = 0.0f;
            }

            return;
        }

        // WASDがある場合
        m_dashTimer += deltaTime;

        if (m_input.moveZ > 0.0f)
        {
            m_dashDirection = 2;
        }
        else if (m_input.moveZ < 0.0f)
        {
            m_dashDirection = 3;
        }
        else if (m_input.moveX > 0.0f)
        {
            m_dashDirection = 0;
        }
        else if (m_input.moveX < 0.0f)
        {
            m_dashDirection = 1;
        }

        m_dashSpeed = DASH_ACCEL_SPEED;

        switch (m_dashDirection)
        {
        case 0:
            m_pos.x += m_dashSpeed;
            m_isReverseX = false;
            break;

        case 1:
            m_pos.x -= m_dashSpeed;
            m_isReverseX = true;
            break;

        case 2:
            m_pos.z += m_dashSpeed;
            break;

        case 3:
            m_pos.z -= m_dashSpeed;
            break;
        }

        if (m_dashTimer >= DASH_STATE_TIME)
        {
            m_dashTimer = DASH_STATE_TIME;
            m_isDash = true;
            m_dashSpeed = DASH_SPEED;
        }

        return;
    }

    // Lを押した瞬間だけ開始
    if (m_input.dash && !m_prevDash)
    {
        m_dashTimer = deltaTime;
        m_dashAccelTimer = 0.0f;

        if (m_input.isMove)
        {
            if (m_input.moveZ > 0.0f)
            {
                m_dashDirection = 2;
            }
            else if (m_input.moveZ < 0.0f)
            {
                m_dashDirection = 3;
            }
            else if (m_input.moveX > 0.0f)
            {
                m_dashDirection = 0;
            }
            else if (m_input.moveX < 0.0f)
            {
                m_dashDirection = 1;
            }
        }
        else
        {
            m_dashDirection = m_lastMoveDirection;
        }

        m_dashSpeed = DASH_ACCEL_SPEED;

        switch (m_dashDirection)
        {
        case 0:
            m_pos.x += m_dashSpeed;
            m_isReverseX = false;
            break;

        case 1:
            m_pos.x -= m_dashSpeed;
            m_isReverseX = true;
            break;

        case 2:
            m_pos.z += m_dashSpeed;
            break;

        case 3:
            m_pos.z -= m_dashSpeed;
            break;
        }
    }
}
void Ninja::Draw()
{
    DrawAnimation();
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

    // 壁キック後は一定時間、
    // キック方向への移動を行う
    m_wallKickInputTimer = WALL_KICK_MOVE_TIME;
}