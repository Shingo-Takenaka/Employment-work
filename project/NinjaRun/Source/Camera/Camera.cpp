#include "Camera.h"

const float CAMERA_HEIGHT = 40.0f;
const float CAMERA_DISTANCE = 100.0f;

Camera::Camera()
{
    // 少し上空から斜めに見る(x, y, z)
    m_offset = VGet(0.0f, 40.0f, -100.0f);

    m_CameraPos = m_offset;

    // 注視点
    m_target = VGet(0.0f, 0.0f, 0.0f);
}

void Camera::Update(VECTOR playerPos)
{
    // 通常時のカメラ追従速度
    const float followSpeed = 0.08f;

    // ジャンプ中のY方向追従速度
    const float jumpFollowSpeed = 0.03f;

    // プレイヤーから見た本来のカメラ位置
    VECTOR targetEye = VAdd(playerPos, m_offset);

    // X・Z方向は通常速度で追従
    m_CameraPos.x += (targetEye.x - m_CameraPos.x) * followSpeed;

    m_CameraPos.z += (targetEye.z - m_CameraPos.z) * followSpeed;

    // Y方向
    // プレイヤーが通常位置より上にいる場合は
    // カメラのY追従を遅くする
    if (playerPos.y > 0.1f)
    {
        m_CameraPos.y += (targetEye.y - m_CameraPos.y) * jumpFollowSpeed;
    }
    else
    {
        m_CameraPos.y += (targetEye.y - m_CameraPos.y) * followSpeed;
    }

    // 注視点
    m_target.x += (playerPos.x - m_target.x) * followSpeed;

    m_target.y += (playerPos.y - m_target.y) * jumpFollowSpeed;

    m_target.z += (playerPos.z - m_target.z) * followSpeed;

    // カメラの位置と注視点を設定
    SetCameraPositionAndTarget_UpVecY(m_CameraPos, m_target);
}