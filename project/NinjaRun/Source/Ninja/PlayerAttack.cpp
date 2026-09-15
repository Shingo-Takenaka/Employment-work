#include "PlayerAttack.h"
#include <math.h>

PlayerAttack::PlayerAttack()
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
float PlayerAttack::GetAttackWidth() const
{
    return m_attackWidth;
}

// Z方向の攻撃範囲
float PlayerAttack::GetAttackDepth() const
{
    return m_attackDepth;
}

// Y方向の攻撃範囲
float PlayerAttack::GetAttackHeight() const
{
    return m_attackHeight;
}

// 手裏剣生成
void PlayerAttack::CreateShoot(
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
void PlayerAttack::UpdateShoot()
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
void PlayerAttack::DrawShoot()
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
bool PlayerAttack::IsShoot() const
{
    return !m_shoots.empty();
}

// 手裏剣の数を取得
int PlayerAttack::GetShootCount() const
{
    return (int)m_shoots.size();
}

// 手裏剣の位置を取得
VECTOR PlayerAttack::GetShootPosition(int index) const
{
    if (index < 0 ||
        index >= (int)m_shoots.size())
    {
        return VGet(0.0f, 0.0f, 0.0f);
    }

    return m_shoots[index].pos;
}

// 手裏剣の向きを取得
bool PlayerAttack::IsShootReverseX(int index) const
{
    if (index < 0 ||
        index >= (int)m_shoots.size())
    {
        return false;
    }

    return m_shoots[index].direction == 1;
}

// 手裏剣を削除
void PlayerAttack::RemoveShoot(int index)
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