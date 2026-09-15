#include "PlayScene.h"
#include "../Collision/TouchEnemyDebug.h"
#include "../Collision/PlayerAttackCollision.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
    m_ninja.Update();

    // Playerの攻撃とEnemyの当たり判定
    if (m_playerAttackCollision.Check(
        m_ninja,
        m_enemy))
    {
        m_enemy.TakeDamage(1);
    }

    // Enemyが死亡していなければ更新
    if (!m_enemy.IsDead())
    {
        m_enemy.Update(m_ninja);
    }

    // Enemyとの接触
    if (!m_enemy.IsDead())
    {
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
    }

    m_camera.Update(
        m_ninja.GetPosition());

    // 手裏剣アニメーションが開始した瞬間に1枚発射
    if (m_ninja.IsShootStart())
    {
        m_playerAttack.CreateShoot(
            m_ninja.GetPosition(),
            m_ninja.IsReverseX());
    }

    // 手裏剣更新
    m_playerAttack.UpdateShoot();

    // 手裏剣とEnemyの当たり判定
    for (int i = m_playerAttack.GetShootCount() - 1; i >= 0; i--)
    {
        if (ShootCollision::Check(
            m_playerAttack,
            i,
            m_enemy))
        {
            // Enemyに1ダメージ
            m_enemy.TakeDamage(1);

            // 当たった手裏剣を削除
            m_playerAttack.RemoveShoot(i);
        }
    }
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

    // 手裏剣登録
    if (m_playerAttack.IsShoot())
    {
        VECTOR playerPos = m_ninja.GetPosition();

        m_drawManager.Add(
            playerPos,
            playerPos.z,
            [&]()
            {
                m_playerAttack.DrawShoot();
            });
    }

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

    // Enemy接触判定デバッグ
    TouchEnemyDebug::Draw(
        m_ninja,
        m_enemy);
}