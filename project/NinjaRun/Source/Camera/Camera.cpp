#include "Camera.h"

const float CAMERA_HEIGHT = 40.0f;
const float CAMERA_DISTANCE = 100.0f;

Camera::Camera()
{
    // 少し上空から斜めに見る(x, y, z)
    m_offset = VGet(0.0f, 40.0f, -100.0f);

    m_eye = m_offset;
    // 注視点
    m_target = VGet(0.0f, 0.0f, 0.0f);
}

void Camera::Update(VECTOR playerPos)
{
    /*const float moveSpeed = 1.0f;

    // ==========================
    // Z軸（前後）
    // ↑キー：前へ
    // ↓キー：後ろへ
    // ==========================
    if (CheckHitKey(KEY_INPUT_UP))
    {
        m_eye.z += moveSpeed;
    }

    if (CheckHitKey(KEY_INPUT_DOWN))
    {
        m_eye.z -= moveSpeed;
    }

    // ==========================
    // Y軸（高さ）
    // →キー：上へ(+)
    // ←キー：下へ(-)
    // ==========================
    if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        m_eye.y += moveSpeed;
    }

    if (CheckHitKey(KEY_INPUT_LEFT))
    {
        m_eye.y -= moveSpeed;
    }

    */

    // プレイヤーに追従
    m_eye = VAdd(playerPos, m_offset);
    m_target = playerPos;

	// カメラの位置と注視点を設定
    SetCameraPositionAndTarget_UpVecY(m_eye, m_target);
}

void Camera::Draw()
{
    // 座標表示
    DrawFormatString(
        10,
        10,
        GetColor(255, 255, 255),
        "Camera Eye : X = %.1f  Y = %.1f  Z = %.1f",
        m_eye.x,
        m_eye.y,
        m_eye.z
    );
    DrawFormatString(
        10,
        30,
        GetColor(255, 255, 255),
        "Target : X = %.1f  Y = %.1f  Z = %.1f",
        m_target.x,
        m_target.y,
        m_target.z
    );
}