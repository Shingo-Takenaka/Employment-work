#include "NormalEnemy.h"

#include "../../Ninja/Ninja.h"

#include <math.h>

NormalEnemy::NormalEnemy()
{
    m_pos = VGet(40.0f, 0.0f, 50.0f);

    m_size = 10.0f;

    // 待機時は右向き
    m_isReverseX = false;

    // HP
    m_hp = 1;

    // 死亡していない
    m_isDead = false;

    // アニメーション読み込み
    m_animation.LoadAnimations();
}

NormalEnemy::~NormalEnemy()
{
}

// 更新
void NormalEnemy::Update(const Ninja& ninja)
{
    // 死亡していたら更新しない
    if (m_isDead)
    {
        return;
    }

    // Playerの座標を保存
    m_playerPos = ninja.GetPosition();

    // Playerの座標
    VECTOR ninjaPos =
        ninja.GetPosition();

    // X・Z方向の距離
    float dx =
        ninjaPos.x - m_pos.x;

    float dz =
        ninjaPos.z - m_pos.z;

    float distance =
        sqrtf(
            dx * dx +
            dz * dz);

    // 射撃判定
    const float shootRange = 50.0f;

    if (distance <= shootRange)
    {
        // 射撃アニメーション
        m_animation.SetAnimation(
            NormalEnemyAnim::SHOOT);

        if (ninjaPos.x < m_pos.x)
        {
            // Playerが左 → 左向き
            m_isReverseX = false;
        }
        else if (ninjaPos.x > m_pos.x)
        {
            // Playerが右 → 右向き
            m_isReverseX = true;
        }
    }
    else
    {
        // 待機
        m_animation.SetAnimation(
            NormalEnemyAnim::WAIT);

        // 待機時は右向き
        m_isReverseX = false;
    }

    // アニメーション更新
    m_animation.Update();

    // 攻撃更新
    m_attack.Update(
        m_pos,
        ninjaPos);
}


// 描画
void NormalEnemy::Draw()
{
    // 死亡していたら描画しない
    if (m_isDead)
    {
        return;
    }

    // 敵本体
    m_animation.DrawAnimation(
        m_pos,
        m_size,
        m_isReverseX);

    // 攻撃エフェクト
    /*m_attack.Draw(
        m_pos,
        m_playerPos);*/
}


// 座標取得
VECTOR NormalEnemy::GetPosition() const
{
    return m_pos;
}


// 座標設定
void NormalEnemy::SetPosition(VECTOR pos)
{
    m_pos = pos;
}


// ダメージを受ける
void NormalEnemy::TakeDamage(int damage)
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
bool NormalEnemy::IsDead() const
{
    return m_isDead;
}