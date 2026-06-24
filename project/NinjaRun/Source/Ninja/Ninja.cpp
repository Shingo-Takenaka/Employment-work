#include "Ninja.h"
#include "../Input/Input.h"

Ninja::Ninja()
{
    m_handle = LoadGraph("Data/Ninja/Wait/Ninja.png");
    m_pos = VGet(0.0f, 0.0f, 0.0f);
    // キャラクターの大きさ
    m_size = 10.0f;

    m_moveSpeed = 0.5f; // 1フレームあたりの移動速度
}

Ninja::~Ninja()
{
    DeleteGraph(m_handle);
}

void Ninja::Update()
{
    // 移動
    // W: 奥(+Z方向), S: 手前(-Z方向), A: 左(-X方向), D: 右(+X方向)
    if (Input::IsPress(KEY_INPUT_W)) { m_pos.z += m_moveSpeed; }
    if (Input::IsPress(KEY_INPUT_S)) { m_pos.z -= m_moveSpeed; }
    if (Input::IsPress(KEY_INPUT_A)) { m_pos.x -= m_moveSpeed; }
    if (Input::IsPress(KEY_INPUT_D)) { m_pos.x += m_moveSpeed; }
}

void Ninja::Draw()
{
    // 3D空間に2D画像を立てて描画する（ビルボード）
    // 第2・3引数(0.5f, 1.0f)は画像のどこを基点にするか
    // 1.0f にすることで、足元(m_pos)を基準に上に伸びるように描画される
    DrawBillboard3D(m_pos, 0.5f, 1.0f, m_size, 0.0f, m_handle, TRUE);
}