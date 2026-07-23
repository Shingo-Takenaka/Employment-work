#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
    m_ninja.Update();

    m_enemy.Update();

    m_camera.Update(m_ninja.GetPosition());
}

void PlayScene::Draw()
{
    // 地面
    m_field.Draw();

    // 描画リストを空にする
    m_drawManager.Clear();

    // プレイヤー登録
    VECTOR playerPos = m_ninja.GetPosition();

    m_drawManager.Add(
        playerPos,
        playerPos.z,
        [&]()
        {
            m_ninja.Draw();
        });

    // 敵登録
    VECTOR enemyPos = m_enemy.GetPosition();

    m_drawManager.Add(
        enemyPos,
        enemyPos.z,
        [&]()
        {
            m_enemy.Draw();
        });

    // ソートして描画
    m_drawManager.Draw();

    // デバッグ
    m_camera.Draw();
}