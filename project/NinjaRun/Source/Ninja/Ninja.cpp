#include "Ninja.h"

#include "../Input/Input.h"
#include <math.h>

Ninja::Ninja()
{

    m_pos = VGet(0.0f, 0.0f, 0.0f);

    m_moveSpeed = 0.5f;

    m_size = 10.0f;

	//待機
    LoadAnimation(
        m_animation[(int)NinjaAnim::WAIT],
        "Data/Ninja/Wait/Wait.png",
        6,   //フレーム数
        32,  //幅
		32,  //高さ
		15); //アニメーションの切り替え間隔

    //歩き
    LoadAnimation(
        m_animation[(int)NinjaAnim::WALK],
        "Data/Ninja/Walk/Walk.png",
		9,   //フレーム数
        32,  //幅
        32,  //高さ
        5);  //アニメーションの切り替え間隔

    m_currentAnim = NinjaAnim::WAIT;

    // 最初は右向き
    m_isReverseX = false;
}

Ninja::~Ninja()
{
    for (int anim = 0; anim < (int)NinjaAnim::MAX; anim++)
    {
        for (int frame = 0; frame < m_animation[anim].frameNum; frame++)
        {
            DeleteGraph(
                m_animation[anim].graph[frame]);
        }
    }
}

bool Ninja::LoadAnimation(
    SpriteAnimation& animation,
    const char* fileName,
    int frameNum,
    int width,
    int height,
    int interval)
{
    animation.frameNum = frameNum;

    if (LoadDivGraph(
        fileName,
        frameNum,
        frameNum,
        1,
        width,
        height,
        animation.graph) == -1)
    {
        return false;
    }

    animation.anim.Init(
        frameNum,
        interval);

    return true;
}

void Ninja::Update()
{
    bool isMove = false;

    // 移動方向
    float moveX = 0.0f;
    float moveZ = 0.0f;

    // キーボード入力
    if (Input::IsPress(KEY_INPUT_A))
    {
        moveX -= 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_D))
    {
        moveX += 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_W))
    {
        moveZ += 1.0f;
    }

    if (Input::IsPress(KEY_INPUT_S))
    {
        moveZ -= 1.0f;
    }

    // コントローラー入力
    DINPUT_JOYSTATE joyState;

    GetJoypadDirectInputState(DX_INPUT_PAD1, &joyState);

    float stickX = joyState.X / 1000.0f;
    float stickY = joyState.Y / 1000.0f;

    // デッドゾーン
    const float DEAD_ZONE = 0.2f;

    if (fabsf(stickX) < DEAD_ZONE)
    {
        stickX = 0.0f;
    }

    if (fabsf(stickY) < DEAD_ZONE)
    {
        stickY = 0.0f;
    }

    // キーボードよりスティックを優先
    if (stickX != 0.0f || stickY != 0.0f)
    {
        moveX = stickX;
        moveZ = -stickY;
    }

    // 斜め移動の速度補正
    float length = sqrtf(moveX * moveX + moveZ * moveZ);

    if (length > 1.0f)
    {
        moveX /= length;
        moveZ /= length;
    }

    // 実際の移動
    if (moveX != 0.0f || moveZ != 0.0f)
    {
        m_pos.x += moveX * m_moveSpeed;
        m_pos.z += moveZ * m_moveSpeed;

        isMove = true;
    }

    // 左右反転
    if (moveX < 0.0f)
    {
        m_isReverseX = true;
    }
    else if (moveX > 0.0f)
    {
        m_isReverseX = false;
    }

    // アニメーション切り替え
    if (isMove)
    {
        if (m_currentAnim != NinjaAnim::WALK)
        {
            m_currentAnim = NinjaAnim::WALK;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }
    else
    {
        if (m_currentAnim != NinjaAnim::WAIT)
        {
            m_currentAnim = NinjaAnim::WAIT;
            m_animation[(int)m_currentAnim].anim.Reset();
        }
    }

    // アニメーション更新
    m_animation[(int)m_currentAnim].anim.Update();
}

void Ninja::Draw()
{
    SpriteAnimation& anim =
        m_animation[(int)m_currentAnim];

    DrawBillboard3D(
        m_pos,  //描画する3D座標
        0.5f,   //横方向の基準
		1.0f,   //縦方向の基準
        m_size, //描画サイズ
		0.0f,   //回転角度
		anim.graph[anim.anim.GetFrame()], //現在のアニメーション画像
		TRUE,   //透過
        m_isReverseX); //左右反転
}

VECTOR Ninja::GetPosition() const
{
    return m_pos;
}