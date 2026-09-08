#include "PlayerAttack.h"

PlayerAttack::PlayerAttack()
{
    // UŒ‚”ÍˆÍ
    m_attackWidth = 15.0f;
    m_attackDepth = 6.0f;
    m_attackHeight = 5.0f;
}

// X•ûŒü‚ÌUŒ‚”ÍˆÍ
float PlayerAttack::GetAttackWidth() const
{
    return m_attackWidth;
}

// Z•ûŒü‚ÌUŒ‚”ÍˆÍ
float PlayerAttack::GetAttackDepth() const
{
    return m_attackDepth;
}

// Y•ûŒü‚ÌUŒ‚”ÍˆÍ
float PlayerAttack::GetAttackHeight() const
{
    return m_attackHeight;
}
