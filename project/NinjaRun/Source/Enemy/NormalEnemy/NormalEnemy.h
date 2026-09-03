#pragma once

#include "DxLib.h"

#include "NormalEnemyAnimation.h"

class Ninja;

class NormalEnemy
{
public:

    NormalEnemy();
    ~NormalEnemy();

    void Update(const Ninja& ninja);
    void Draw();

    // 座標取得
    VECTOR GetPosition() const;

    // 座標設定
    void SetPosition(VECTOR pos);

private:

    // 基本情報
    VECTOR m_pos;

    float m_size;

    // 左右反転
    bool m_isReverseX;

    // アニメーション
    NormalEnemyAnimation m_animation;
};