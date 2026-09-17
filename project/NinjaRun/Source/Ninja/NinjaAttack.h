#pragma once

#include "DxLib.h"
#include <vector>

class NinjaAttack
{
public:

    NinjaAttack();

    // 攻撃範囲
    float GetAttackWidth() const;
    float GetAttackDepth() const;
    float GetAttackHeight() const;

    // 手裏剣生成
    void CreateShoot(
        VECTOR pos,
        int direction
    );

    // 手裏剣更新
    void UpdateShoot();

    // 手裏剣描画
    void DrawShoot();

    // 手裏剣が存在するか
    bool IsShoot() const;

    // 手裏剣の数
    int GetShootCount() const;

    // 手裏剣の位置
    VECTOR GetShootPosition(int index) const;

    // 手裏剣の向き
    bool IsShootReverseX(int index) const;

    // 手裏剣を削除
    void RemoveShoot(int index);

private:

    float m_attackWidth;
    float m_attackDepth;
    float m_attackHeight;

    int m_shootGraph;

    float m_shootSpeed;
    float m_shootMaxDistance;
    float m_shootSize;

    struct Shoot
    {
        VECTOR pos;
        VECTOR startPos;

        // 発射方向
        // 0 = +X（D）
        // 1 = -X（A）
        // 2 = +Z（W）
        // 3 = -Z（S）
        int direction;
    };

    std::vector<Shoot> m_shoots;
};