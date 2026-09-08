#pragma once

class PlayerAttack
{
public:

    PlayerAttack();

    // UŒ‚”ÍˆÍæ“¾
    float GetAttackWidth() const;
    float GetAttackDepth() const;
    float GetAttackHeight() const;

private:

    // UŒ‚”ÍˆÍ
    float m_attackWidth;
    float m_attackDepth;
    float m_attackHeight;
};
