#include "EnemyBase.h"

EnemyBase::EnemyBase(int hp)
{
    m_pos = VGet(0.0f, 0.0f, 0.0f);

    m_size = 10.0f;

    // 初期状態では右向き
    m_isReverseX = false;

    // HP
    m_hp = hp;

    // 死亡していない
    m_isDead = false;
}

EnemyBase::~EnemyBase()
{
}

// 座標取得
VECTOR EnemyBase::GetPosition() const
{
    return m_pos;
}

// 座標設定
void EnemyBase::SetPosition(VECTOR pos)
{
    m_pos = pos;
}

// ダメージを受ける
void EnemyBase::TakeDamage(int damage)
{
    if (m_isDead)
    {
        return;
    }

    m_hp -= damage;

    if (m_hp <= 0)
    {
        m_hp = 0;
        m_isDead = true;
    }
}

// 死亡しているか
bool EnemyBase::IsDead() const
{
    return m_isDead;
}