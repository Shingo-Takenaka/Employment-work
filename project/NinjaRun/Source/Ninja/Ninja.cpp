#include "Ninja.h"

namespace
{
    // ダッシュ状態になるまでの時間
    const float DASH_STATE_TIME = 0.4f;

    // Lを1回押したときの加速距離
    const float DASH_ACCEL_DISTANCE = 10.0f;

    // L + WASDでダッシュしているときの速度
    const float DASH_SPEED = 1.0f;
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

    // ダッシュ方向
    m_dashDirection = 0;

    // ダッシュ
    m_isDash = false;
    m_dashTimer = 0.0f;
    m_dashSpeed = DASH_SPEED;

    // ジャンプ
    m_isJump = false;

    m_groundY = m_pos.y;

    m_jumpSpeed = 0.0f;

    m_gravity = 0.05f;

    // 壁キック
    m_canWallKick = false;

    m_wallKickDirection =
        VGet(0.0f, 0.0f, 0.0f);

    // 攻撃
    m_isSlash = false;
    m_isShoot = false;

    // ガード
    m_isGuard = false;

    // ノックバック
    m_isKnockback = false;

    m_knockbackDirection =
        VGet(0.0f, 0.0f, 0.0f);

    m_knockbackStrength = 0.0f;

    m_knockbackTimer = 0.0f;

    // 手裏剣発射タイミング
    m_isShootStart = false;

    // 最初は右方向
    m_lastShootDirection = 0;
}

Ninja::~Ninja()
{
    for (int anim = 0;
        anim < (int)NinjaAnim::MAX;
        anim++)
    {
        for (int frame = 0;
            frame < m_animation[anim].frameNum;
            frame++)
        {
            DeleteGraph(
                m_animation[anim].graph[frame]);
        }
    }
}

void Ninja::Update()
{
    // 手裏剣を投げたタイミングを毎フレームリセット
    m_isShootStart = false;

    // ノックバック中なら通常操作を行わない
    if (m_isKnockback)
    {
        // ダッシュを解除
        m_isDash = false;
        m_dashTimer = 0.0f;

        m_pos.x +=
            m_knockbackDirection.x *
            m_knockbackStrength;

        m_pos.z +=
            m_knockbackDirection.z *
            m_knockbackStrength;

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

    // ダッシュ処理
    UpdateDash();

    // ダッシュ中は通常移動を行わない
    if (m_isDash)
    {
        UpdateAnimation(false);

        return;
    }

    // Lを押している間はダッシュ処理だけを行う
    if (m_input.dash)
    {
        UpdateAnimation(false);

        return;
    }

    // ガード
    if (m_input.guard &&
        !m_isJump &&
        !m_isSlash &&
        !m_isShoot)
    {
        m_isGuard = true;
    }
    else
    {
        m_isGuard = false;
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

    // 近接攻撃
    if (m_input.slash &&
        !m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
    {
        m_isSlash = true;

        m_currentAnim = NinjaAnim::SLASH;

        m_animation[
            (int)NinjaAnim::SLASH
        ].anim.Reset();
    }

    // 遠距離攻撃
    if (m_input.shoot &&
        !m_isShoot &&
        !m_isGuard &&
        !m_isSlash)
    {
        // 手裏剣攻撃開始
        m_isShoot = true;

        // SHOOTアニメーションを必ず最初から再生
        m_currentAnim = NinjaAnim::SHOOT;

        m_animation[
            (int)NinjaAnim::SHOOT
        ].anim.Reset();

        // このフレームで手裏剣を1個発射
        m_isShootStart = true;
    }

    // 移動
    if (!m_isSlash &&
        !m_isGuard &&
        !m_isShoot)
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
    // LもWASDも押していない場合
    if (!m_input.dash && !m_input.isMove)
    {
        m_isDash = false;
        m_dashTimer = 0.0f;

        return;
    }

    // まだダッシュ状態ではない場合
    if (!m_isDash)
    {
        // L + WASD
        if (m_input.dash && m_input.isMove)
        {
            // 現在押している方向を取得
            if (m_input.moveZ > 0.0f)
            {
                // W
                m_dashDirection = 2;
            }
            else if (m_input.moveZ < 0.0f)
            {
                // S
                m_dashDirection = 3;
            }
            else if (m_input.moveX > 0.0f)
            {
                // D
                m_dashDirection = 0;
            }
            else if (m_input.moveX < 0.0f)
            {
                // A
                m_dashDirection = 1;
            }

            // ダッシュ時間を加算
            m_dashTimer += 1.0f / 60.0f;

            // ダッシュ移動
            switch (m_dashDirection)
            {
            case 0:
                // +X
                m_pos.x += m_dashSpeed;
                m_isReverseX = false;
                break;

            case 1:
                // -X
                m_pos.x -= m_dashSpeed;
                m_isReverseX = true;
                break;

            case 2:
                // +Z
                m_pos.z += m_dashSpeed;
                break;

            case 3:
                // -Z
                m_pos.z -= m_dashSpeed;
                break;
            }

            // 1秒以上ならダッシュ状態
            if (m_dashTimer >= DASH_STATE_TIME)
            {
                m_isDash = true;
            }

            return;
        }

        // Lだけ
        if (m_input.dash && !m_input.isMove)
        {
            // 最初の1回だけ加速
            if (m_dashTimer <= 0.0f)
            {
                m_dashDirection = m_lastMoveDirection;

                switch (m_dashDirection)
                {
                case 0:
                    // +X
                    m_pos.x += DASH_ACCEL_DISTANCE;
                    m_isReverseX = false;
                    break;

                case 1:
                    // -X
                    m_pos.x -= DASH_ACCEL_DISTANCE;
                    m_isReverseX = true;
                    break;

                case 2:
                    // +Z
                    m_pos.z += DASH_ACCEL_DISTANCE;
                    break;

                case 3:
                    // -Z
                    m_pos.z -= DASH_ACCEL_DISTANCE;
                    break;
                }

                // Lを押したことを記録
                m_dashTimer = 1.0f;
            }

            return;
        }
    }

    // ダッシュ状態中
    if (m_isDash)
    {
        // WASDを離したらダッシュ終了
        if (!m_input.isMove)
        {
            m_isDash = false;
            m_dashTimer = 0.0f;

            return;
        }

        // 現在押している方向に方向転換
        if (m_input.moveZ > 0.0f)
        {
            // W
            m_dashDirection = 2;
        }
        else if (m_input.moveZ < 0.0f)
        {
            // S
            m_dashDirection = 3;
        }
        else if (m_input.moveX > 0.0f)
        {
            // D
            m_dashDirection = 0;
        }
        else if (m_input.moveX < 0.0f)
        {
            // A
            m_dashDirection = 1;
        }

        // ダッシュ速度で移動
        switch (m_dashDirection)
        {
        case 0:
            // +X
            m_pos.x += m_dashSpeed;
            m_isReverseX = false;
            break;

        case 1:
            // -X
            m_pos.x -= m_dashSpeed;
            m_isReverseX = true;
            break;

        case 2:
            // +Z
            m_pos.z += m_dashSpeed;
            break;

        case 3:
            // -Z
            m_pos.z -= m_dashSpeed;
            break;
        }

        return;
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

    // 壁から離れる方向へ移動
    m_pos.x +=
        m_wallKickDirection.x * 1.0f;

    m_pos.z +=
        m_wallKickDirection.z * 1.0f;

    // 上方向へ強く飛ぶ
    m_jumpSpeed = 1.5f;

    // m_groundYは変更しない
    // 壁キック前のジャンプ開始地点を着地点として使う
    m_isJump = true;

    // 壁キック権を消費
    m_canWallKick = false;
}