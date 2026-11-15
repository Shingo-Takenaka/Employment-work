
#include "Field.h"

Field::Field()
    : m_model(-1)
{
    m_fieldPos = VGet(0.0f, 0.0f, 0.0f);
    m_fieldRot = VGet(0.0f, 0.0f, 0.0f);
    m_fieldScale = VGet(0.25f, 0.25f, 0.25f);

    Load();
}

Field::~Field()
{
    if (m_model >= 0)
    {
        MV1DeleteModel(m_model);
    }
}

void Field::Load()
{
    m_model = MV1LoadModel("Data/Field/test/Map.x");

    if (m_model < 0)
    {
        return;
    }

    SetupModel();

    // ƒ‚ƒfƒ‹‘S‘Ì‚Ì“–‚½‚è”»’èî•ñ‚ðì¬
    MV1SetupCollInfo(m_model, -1);
}

void Field::SetupModel()
{
    if (m_model < 0)
    {
        return;
    }

    MV1SetPosition(m_model, m_fieldPos);
    MV1SetRotationXYZ(m_model, m_fieldRot);
    MV1SetScale(m_model, m_fieldScale);
}

void Field::Draw()
{
    if (m_model < 0)
    {
        return;
    }

    MV1DrawModel(m_model);
}

int Field::GetModelHandle() const
{
    return m_model;
}
