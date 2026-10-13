#include "NinjaAttack.h"
#include <math.h>

NinjaAttack::NinjaAttack()
{
    // 攻撃範囲
    m_attackWidth = 15.0f;
    m_attackDepth = 6.0f;
    m_attackHeight = 5.0f;

    // 手裏剣
    m_shootGraph = LoadGraph(
        "Data/Ninja/Shoot/Shoot.png"
    );

    // 手裏剣の移動速度
    m_shootSpeed = 1.0f;

    // 手裏剣の最大飛距離
    m_shootMaxDistance = 50.0f;

    // 手裏剣のサイズ
    m_shootSize = 1.0f;
}

// X方向の攻撃範囲
float NinjaAttack::GetAttackWidth() const
{
    return m_attackWidth;
}

// Z方向の攻撃範囲
float NinjaAttack::GetAttackDepth() const
{
    return m_attackDepth;
}

// Y方向の攻撃範囲
float NinjaAttack::GetAttackHeight() const
{
    return m_attackHeight;
}

// 手裏剣生成
void NinjaAttack::CreateShoot(
    VECTOR pos,
    int direction)
{
    // 発射位置を少し上にする
    pos.y += 3.0f;

    // 新しい手裏剣を作成
    Shoot shoot;

    // 発射位置
    shoot.pos = pos;

    // 最大飛距離を計算するための開始位置
    shoot.startPos = pos;

    // 発射方向
    shoot.direction = direction;

    // 手裏剣を追加
    m_shoots.push_back(shoot);
}

// 手裏剣更新
void NinjaAttack::UpdateShoot()
{
    // すべての手裏剣を更新
    for (int i = (int)m_shoots.size() - 1; i >= 0; i--)
    {
        Shoot& shoot = m_shoots[i];

        // +X方向
        if (shoot.direction == 0)
        {
            shoot.pos.x += m_shootSpeed;
        }
        // -X方向
        else if (shoot.direction == 1)
        {
            shoot.pos.x -= m_shootSpeed;
        }
        // +Z方向
        else if (shoot.direction == 2)
        {
            shoot.pos.z += m_shootSpeed;
        }
        // -Z方向
        else if (shoot.direction == 3)
        {
            shoot.pos.z -= m_shootSpeed;
        }

        // 発射位置からの距離
        float dx =
            shoot.pos.x -
            shoot.startPos.x;

        float dz =
            shoot.pos.z -
            shoot.startPos.z;

        float distance =
            sqrtf(
                dx * dx +
                dz * dz
            );

        // 最大飛距離に到達したら削除
        if (distance >= m_shootMaxDistance)
        {
            m_shoots.erase(
                m_shoots.begin() + i
            );
        }
    }
}

// 手裏剣描画
void NinjaAttack::DrawShoot()
{
    // すべての手裏剣を描画
    for (const Shoot& shoot : m_shoots)
    {
        // 左方向の場合だけ画像を反転
        bool reverseX = false;

        if (shoot.direction == 1)
        {
            reverseX = true;
        }

        DrawBillboard3D(
            shoot.pos,
            0.5f,
            0.5f,
            m_shootSize,
            0.0f,
            m_shootGraph,
            TRUE,
            reverseX
        );
    }
}

// 手裏剣が存在するか
bool NinjaAttack::IsShoot() const
{
    return !m_shoots.empty();
}

// 手裏剣の数を取得
int NinjaAttack::GetShootCount() const
{
    return (int)m_shoots.size();
}

// 手裏剣の位置を取得
VECTOR NinjaAttack::GetShootPosition(int index) const
{
    if (index < 0 ||
        index >= (int)m_shoots.size())
    {
        return VGet(0.0f, 0.0f, 0.0f);
    }

    return m_shoots[index].pos;
}

// 手裏剣の向きを取得
bool NinjaAttack::IsShootReverseX(int index) const
{
    if (index < 0 ||
        index >= (int)m_shoots.size())
    {
        return false;
    }

    return m_shoots[index].direction == 1;
}

// 手裏剣を削除
void NinjaAttack::RemoveShoot(int index)
{
    if (index < 0 ||
        index >= (int)m_shoots.size())
    {
        return;
    }

    m_shoots.erase(
        m_shoots.begin() + index
    );
}

bool NinjaAttack::StartDashCut(int direction)
{
    // 発動中やクールタイム中は開始しない
    if (m_isDashCutActive || !CanStartDashCut())
    {
        return false;
    }

    // 方向を保存する
    m_dashCutDirection = direction;

    // 発動開始時刻を記録する
    m_dashCutStartTime = GetNowCount();

    // ダッシュ斬りを開始する
    m_isDashCutActive = true;

    // 連続発動回数を増やす
    m_dashCutComboCount++;

    return true;
}

void NinjaAttack::UpdateDashCut()
{
    if (!m_isDashCutActive)
    {
        return;
    }

    // 0.1秒経過したら終了する
    if (GetNowCount() - m_dashCutStartTime >= DASH_CUT_DURATION)
    {
        m_isDashCutActive = false;

        // 3回目の終了時点からクールタイムを開始する
        if (m_dashCutComboCount >= DASH_CUT_MAX_COUNT)
        {
            m_isDashCutCooldown = true;
            m_dashCutCooldownStartTime = GetNowCount();
        }
    }
}

bool NinjaAttack::IsDashCutActive() const
{
    return m_isDashCutActive;
}

bool NinjaAttack::CanStartDashCut()
{
    // クールタイム中か確認する
    if (m_isDashCutCooldown)
    {
        // 3秒経過したらクールタイムを解除する
        if (GetNowCount() - m_dashCutCooldownStartTime
            >= DASH_CUT_COOLDOWN)
        {
            m_isDashCutCooldown = false;
            m_dashCutComboCount = 0;
        }
        else
        {
            return false;
        }
    }

    // 最大発動回数に達していない場合は発動可能
    return m_dashCutComboCount < DASH_CUT_MAX_COUNT;
}

int NinjaAttack::GetDashCutDirection() const
{
    return m_dashCutDirection;
}