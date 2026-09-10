#pragma once

#include "DxLib.h"
#include <vector>

class PlayerAttack
{
public:

    PlayerAttack();

    // 攻撃範囲取得
    float GetAttackWidth() const;
    float GetAttackDepth() const;
    float GetAttackHeight() const;

    // 手裏剣
    void CreateShoot(VECTOR pos, bool isReverseX);
    void UpdateShoot();
    void DrawShoot();

    // 手裏剣が存在するか
    bool IsShoot() const;

private:

    // 手裏剣1個分の情報
    struct Shoot
    {
        VECTOR pos;
        VECTOR startPos;

        bool reverseX;
    };

    // 攻撃範囲
    float m_attackWidth;
    float m_attackDepth;
    float m_attackHeight;

    // 手裏剣
    int m_shootGraph;

    // 手裏剣の移動速度
    float m_shootSpeed;

    // 手裏剣の最大飛距離
    float m_shootMaxDistance;

    // 手裏剣のサイズ
    float m_shootSize;

    // 発射中の手裏剣
    std::vector<Shoot> m_shoots;
};