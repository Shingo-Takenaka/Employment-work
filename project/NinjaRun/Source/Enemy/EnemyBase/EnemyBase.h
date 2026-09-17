#pragma once

#include "DxLib.h"

class Ninja;

class EnemyBase
{
public:
    EnemyBase(int hp);
    virtual ~EnemyBase();

    // 更新
    virtual void Update(const Ninja& ninja) = 0;

    // 描画
    virtual void Draw() = 0;

    // 座標取得
    VECTOR GetPosition() const;

    // 座標設定
    void SetPosition(VECTOR pos);

    // ダメージを受ける
    void TakeDamage(int damage);

    // 死亡しているか
    bool IsDead() const;

protected:
    // 敵の座標
    VECTOR m_pos;

    // 敵の大きさ
    float m_size;

    // 敵の向き
    bool m_isReverseX;

    // HP
    int m_hp;

    // 死亡しているか
    bool m_isDead;
};