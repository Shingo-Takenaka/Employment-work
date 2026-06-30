#include "Ninja.h"
#include "../Input/Input.h"

Ninja::Ninja()
{
    m_pos = VGet(0.0f, 0.0f, 0.0f);

    m_size = 10.0f;
    m_moveSpeed = 0.5f;

    // アニメーション画像を分割
    if (LoadDivGraph(
        "Data/Ninja/Wait/Wait.png",
        WAIT_FRAME,
        WAIT_FRAME,
        1,
        32,
        32,
        m_waitGraph) == -1)
    {
        printfDx("LoadDivGraph Error");
    }

    m_waitAnim.Init(WAIT_FRAME, 15);
}

Ninja::~Ninja()
{
    for (int i = 0; i < WAIT_FRAME; i++)
    {
        DeleteGraph(m_waitGraph[i]);
    }
}

void Ninja::Update()
{
    if (Input::IsPress(KEY_INPUT_W))
    {
        m_pos.z += m_moveSpeed;
    }

    if (Input::IsPress(KEY_INPUT_S))
    {
        m_pos.z -= m_moveSpeed;
    }

    if (Input::IsPress(KEY_INPUT_A))
    {
        m_pos.x -= m_moveSpeed;
    }

    if (Input::IsPress(KEY_INPUT_D))
    {
        m_pos.x += m_moveSpeed;
    }

    m_waitAnim.Update();
}

void Ninja::Draw()
{
    DrawBillboard3D(
        m_pos, 
        0.5f,
        1.0f,
        m_size,
        0.0f,
        m_waitGraph[m_waitAnim.GetFrame()],
        TRUE);
}

VECTOR Ninja::GetPosition() const
{
    return m_pos;
}