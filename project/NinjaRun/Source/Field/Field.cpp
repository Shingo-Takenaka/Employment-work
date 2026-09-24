#include "Field.h"

Field::Field()
{
    // フィールド
    m_model = MV1LoadModel("Data/Field/TestFieldHwite.x");

    // 壁
    m_wall = MV1LoadModel("Data/Field/Wall.x");

    // フィールドの位置・回転・大きさ
    m_fieldPos = VGet(0, 0, 0);
    m_fieldRot = VGet(0, 0, 0);
    m_fieldScale = VGet(1, 1, 1);

    // 壁の位置・回転・大きさ
    m_wallPos = VGet(-60, 30, 0);
    m_wallRot = VGet(0, 0, 0);
    m_wallScale = VGet(1, 1, 1);
}

Field::~Field()
{
    MV1DeleteModel(m_model);
    MV1DeleteModel(m_wall);
}

void Field::Draw()
{
    // フィールド
    MV1SetPosition(m_model, m_fieldPos);
    MV1SetRotationXYZ(m_model, m_fieldRot);
    MV1SetScale(m_model, m_fieldScale);

    MV1DrawModel(m_model);

    // 壁
    MV1SetPosition(m_wall, m_wallPos);
    MV1SetRotationXYZ(m_wall, m_wallRot);
    MV1SetScale(m_wall, m_wallScale);

    MV1DrawModel(m_wall);
}

VECTOR Field::GetWallPosition() const
{
    return m_wallPos;
}