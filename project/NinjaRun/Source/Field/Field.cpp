#include "Field.h"

Field::Field()
{
    //デモマップ
    // m_model = MV1LoadModel("Data/Field/DemoMap.x");

    //描画テスト
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
    //デモフィールドの描画テスト用だったもの(家で詳しく調べるため一旦保留)
    //MV1SetLightEnable(m_model, FALSE);

    MV1SetPosition(m_model, m_pos);

    MV1SetRotationXYZ(m_model, m_rot);

    MV1SetScale(m_model, m_scale);

    MV1DrawModel(m_model);
}