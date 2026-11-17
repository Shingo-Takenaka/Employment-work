
#include "MoveFloor.h"

namespace
{
    const char* MODEL_PATH = "Data/Field/test/TestFieldHwite.x";

    const VECTOR START_POSITION = VGet(40.0f, 0.0f, 0.0f);
    const VECTOR END_POSITION = VGet(40.0f, 30.0f, 0.0f);

    const float MOVE_SPEED = 0.2f;

    // 移動床のサイズ
    const VECTOR FLOOR_SCALE = VGet(0.3f, 0.3f, 0.3f);
}

MoveFloor::MoveFloor()
    : m_modelHandle(-1),
    m_position(VGet(0.0f, 0.0f, 0.0f)),
    m_startPosition(VGet(0.0f, 0.0f, 0.0f)),
    m_endPosition(VGet(0.0f, 0.0f, 0.0f)),
    m_moveAmount(VGet(0.0f, 0.0f, 0.0f)),
    m_speed(0.0f),
    m_isMovingToEnd(true)
{
}

MoveFloor::~MoveFloor()
{
    if (m_modelHandle >= 0)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }
}

void MoveFloor::Init()
{
    m_startPosition = START_POSITION;
    m_endPosition = END_POSITION;
    m_position = m_startPosition;

    m_speed = MOVE_SPEED;
    m_isMovingToEnd = true;

    m_moveAmount = VGet(0.0f, 0.0f, 0.0f);

    if (m_modelHandle >= 0)
    {
        MV1SetPosition(m_modelHandle, m_position);
    }
}

bool MoveFloor::Load()
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

    // モデルのサイズを設定
    MV1SetScale(m_modelHandle, FLOOR_SCALE);

    // モデルの位置を設定
    MV1SetPosition(m_modelHandle, m_position);

    return true;

    MV1SetupCollInfo(m_modelHandle, -1);
}

void MoveFloor::Update()
{
    VECTOR oldPosition = m_position;

    VECTOR targetPosition =
        m_isMovingToEnd ? m_endPosition : m_startPosition;

    VECTOR direction = VSub(targetPosition, m_position);

    float distance = VSize(direction);

    if (m_speed > 0.0f)
    {
        if (distance <= m_speed)
        {
            m_position = targetPosition;
            m_isMovingToEnd = !m_isMovingToEnd;
        }
        else
        {
            direction = VScale(direction, 1.0f / distance);

            m_position = VAdd(
                m_position,
                VScale(direction, m_speed));
        }
    }

    m_moveAmount = VSub(m_position, oldPosition);

    if (m_modelHandle >= 0)
    {
        MV1SetPosition(m_modelHandle, m_position);

        // 移動後のポリゴン当たり判定情報を更新
        MV1RefreshCollInfo(m_modelHandle, -1);
    }
}

void MoveFloor::Draw() const
{
    if (m_modelHandle < 0)
    {
        return;
    }

    MV1DrawModel(m_modelHandle);
}
