#include "Field.h"

Field::Field()
{
    m_model = MV1LoadModel("Data/Field/TestFieldR.x");

    m_pos = VGet(0, 0, 0);

    m_rot = VGet(0, 0, 0);

    m_scale = VGet(1, 1, 1);
}

Field::~Field()
{
    MV1DeleteModel(m_model);
}

void Field::Draw()
{
    MV1SetPosition(m_model, m_pos);

    MV1SetRotationXYZ(m_model, m_rot);

    MV1SetScale(m_model, m_scale);

    MV1DrawModel(m_model);
}