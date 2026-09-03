#include "PlayScene.h"
#include "../Collision/TouchEnemyDebug.h"

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

    // Enemyとの接触
    if (TouchEnemy::Check(
        m_ninja,
        m_enemy))
    {
        if (!m_ninja.IsKnockback())
        {
            TouchEnemy::Apply(
                m_ninja,
                m_enemy);
        }
    }

    m_camera.Update(
        m_ninja.GetPosition());
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
    
    //// Enemy接触判定デバッグ
    //DrawFormatString(
    //    10,
    //    10,
    //    GetColor(255, 255, 255),
    //    "PlayerとEnemyの接触：%s",
    //    m_isTouchEnemy ? "true" : "false"
    //);

    // 当たり判定デバッグ
    TouchEnemyDebug::Draw(
        m_ninja,
        m_enemy);
}