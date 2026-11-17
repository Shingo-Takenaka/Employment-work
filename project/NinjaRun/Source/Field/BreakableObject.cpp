#include "BreakableObject.h"

namespace
{
    const char* MODEL_PATH = "Data/Field/test/Wall.x";

    const VECTOR OBJECT_POSITION = VGet(0.0f, 0.0f, 0.0f);

    const int OBJECT_MAX_HP = 10;
}

BreakableObject::BreakableObject()
    : m_modelHandle(-1),
    m_position(VGet(0.0f, 0.0f, 0.0f)),
    m_hp(0),
    m_maxHP(0),
    m_isBroken(false)
{
}

BreakableObject::~BreakableObject()
{
    if (m_modelHandle >= 0)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }
}

void BreakableObject::Init()
{
    m_position = OBJECT_POSITION;

    m_maxHP = OBJECT_MAX_HP;
    m_hp = m_maxHP;

    m_isBroken = false;

    if (m_modelHandle >= 0)
    {
        MV1SetPosition(m_modelHandle, m_position);
    }
}

bool BreakableObject::Load()
{
    if (m_modelHandle >= 0)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }

    m_modelHandle = MV1LoadModel(MODEL_PATH);

    if (m_modelHandle < 0)
    {
        return false;
    }

    MV1SetPosition(m_modelHandle, m_position);

    return true;
}

void BreakableObject::Draw() const
{
    if (m_isBroken)
    {
        return;
    }

    if (m_modelHandle < 0)
    {
        return;
    }

    MV1DrawModel(m_modelHandle);
}

void BreakableObject::TakeDamage(int damage)
{
    if (m_isBroken || damage <= 0)
    {
        return;
    }

    m_hp -= damage;

    if (m_hp <= 0)
    {
        m_hp = 0;
        m_isBroken = true;
    }
}
