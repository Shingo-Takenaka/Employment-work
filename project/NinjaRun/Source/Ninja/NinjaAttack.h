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

    // ダッシュ斬り
    bool StartDashCut(int direction);
    void UpdateDashCut();
    bool IsDashCutActive() const;
    bool CanStartDashCut();
    int GetDashCutDirection() const;

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

    // ダッシュ斬りの設定
    static const int DASH_CUT_DURATION = 500; //移動している時間(0.5秒)
    static const int DASH_CUT_COOLDOWN = 3000;//クールタイム(3秒)
    static const int DASH_CUT_MAX_COUNT = 3;  //最大回数

    // ダッシュ斬りの状態
    bool m_isDashCutActive;
    int m_dashCutDirection;
    int m_dashCutStartTime;

    // 連続発動とクールタイム
    int m_dashCutComboCount;
    bool m_isDashCutCooldown;
    int m_dashCutCooldownStartTime;

    std::vector<Shoot> m_shoots;
};