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
    // 通常時のカメラ追従速度
    const float followSpeed = 0.08f;

    // ジャンプ中のY方向追従速度
    const float jumpFollowSpeed = 0.03f;

    // プレイヤーから見た本来のカメラ位置
    VECTOR targetEye =
        VAdd(playerPos, m_offset);

    // X・Z方向は通常速度で追従
    m_eye.x +=
        (targetEye.x - m_eye.x) * followSpeed;

    m_eye.z +=
        (targetEye.z - m_eye.z) * followSpeed;

    // Y方向
    // プレイヤーが通常位置より上にいる場合は
    // カメラのY追従を遅くする
    if (playerPos.y > 0.1f)
    {
        m_eye.y +=
            (targetEye.y - m_eye.y) * jumpFollowSpeed;
    }
    else
    {
        m_eye.y +=
            (targetEye.y - m_eye.y) * followSpeed;
    }

    // 注視点
    m_target.x +=
        (playerPos.x - m_target.x) * followSpeed;

    m_target.y +=
        (playerPos.y - m_target.y) * jumpFollowSpeed;

    m_target.z +=
        (playerPos.z - m_target.z) * followSpeed;

    // カメラの位置と注視点を設定
    SetCameraPositionAndTarget_UpVecY(
        m_eye,
        m_target);
}

void Camera::Draw()
{
    //// 座標表示
    //DrawFormatString(
    //    10,
    //    10,
    //    GetColor(255, 255, 255),
    //    "Camera Eye : X = %.1f  Y = %.1f  Z = %.1f",
    //    m_eye.x,
    //    m_eye.y,
    //    m_eye.z
    //);
    //DrawFormatString(
    //    10,
    //    30,
    //    GetColor(255, 255, 255),
    //    "Target : X = %.1f  Y = %.1f  Z = %.1f",
    //    m_target.x,
    //    m_target.y,
    //    m_target.z
    //);
}