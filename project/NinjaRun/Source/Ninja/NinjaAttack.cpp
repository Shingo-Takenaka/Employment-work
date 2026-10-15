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
        "Data/Ninja/Attack/Shoot/Shoot.png"
    );

    // 手裏剣の移動速度
    m_shootSpeed = 1.0f;

    // 手裏剣の最大飛距離
    m_shootMaxDistance = 50.0f;

    // 手裏剣のサイズ
    m_shootSize = 1.0f;

    // ダッシュ斬り
    m_isDashCutActive = false;
    m_dashCutDirectionX = 1.0f;
    m_dashCutDirectionZ = 0.0f;
    m_dashCutStartTime = 0;

    // 連続発動とクールタイム
    m_dashCutComboCount = 0;
    m_isDashCutCooldown = false;
    m_dashCutCooldownStartTime = 0;
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
    float directionX = 0.0f;
    float directionZ = 0.0f;

    // 従来の4方向指定にも対応
    switch (direction)
    {
    case 0:
        directionX = 1.0f;
        break;

    case 1:
        directionX = -1.0f;
        break;

    case 2:
        directionZ = 1.0f;
        break;

    case 3:
        directionZ = -1.0f;
        break;

    default:
        return false;
    }

    return StartDashCut(directionX, directionZ);
}

bool NinjaAttack::StartDashCut(float directionX, float directionZ)
{
    if (m_isDashCutActive || !CanStartDashCut())
    {
        return false;
    }

    // 入力がない場合は発動しない
    float length = sqrtf(
        directionX * directionX +
        directionZ * directionZ
    );

    if (length <= 0.001f)
    {
        return false;
    }

    // 斜め方向でも速度が一定になるように正規化
    m_dashCutDirectionX = directionX / length;
    m_dashCutDirectionZ = directionZ / length;

    m_dashCutStartTime = GetNowCount();
    m_isDashCutActive = true;

    m_dashCutComboCount++;

    return true;
}

void NinjaAttack::UpdateDashCut()
{
    if (!m_isDashCutActive)
    {
        return;
    }

    if (GetNowCount() - m_dashCutStartTime >= DASH_CUT_DURATION)
    {
        m_isDashCutActive = false;

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
    if (m_isDashCutCooldown)
    {
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

    return m_dashCutComboCount < DASH_CUT_MAX_COUNT;
}

int NinjaAttack::GetDashCutDirection() const
{
    // 従来の4方向取得との互換性を維持
    if (fabsf(m_dashCutDirectionX) >=
        fabsf(m_dashCutDirectionZ))
    {
        return m_dashCutDirectionX >= 0.0f ? 0 : 1;
    }

    return m_dashCutDirectionZ >= 0.0f ? 2 : 3;
}

float NinjaAttack::GetDashCutDirectionX() const
{
    return m_dashCutDirectionX;
}

float NinjaAttack::GetDashCutDirectionZ() const
{
    return m_dashCutDirectionZ;
}
