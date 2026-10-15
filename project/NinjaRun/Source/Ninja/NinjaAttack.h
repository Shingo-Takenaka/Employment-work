
#pragma once

#include "DxLib.h"
#include <vector>

class NinjaAttack
{
public:
    NinjaAttack();

    float GetAttackWidth() const;
    float GetAttackDepth() const;
    float GetAttackHeight() const;

    void CreateShoot(VECTOR pos, int direction);
    void UpdateShoot();
    void DrawShoot();

    bool IsShoot() const;
    int GetShootCount() const;
    VECTOR GetShootPosition(int index) const;
    bool IsShootReverseX(int index) const;
    void RemoveShoot(int index);

    // ダッシュ斬り
    bool StartDashCut(int direction);
    bool StartDashCut(float directionX, float directionZ);

    void UpdateDashCut();
    bool IsDashCutActive() const;
    bool CanStartDashCut();

    int GetDashCutDirection() const;
    float GetDashCutDirectionX() const;
    float GetDashCutDirectionZ() const;

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
        int direction;
    };

    static const int DASH_CUT_DURATION = 250;
    static const int DASH_CUT_COOLDOWN = 3000;
    static const int DASH_CUT_MAX_COUNT = 3;

    bool m_isDashCutActive;

    // ダッシュ斬りの方向
    float m_dashCutDirectionX;
    float m_dashCutDirectionZ;

    int m_dashCutStartTime;

    int m_dashCutComboCount;
    bool m_isDashCutCooldown;
    int m_dashCutCooldownStartTime;

    std::vector<Shoot> m_shoots;
};
