#include "PlayScene.h"

#include "../Collision/TouchEnemyDebug.h"
#include "../Collision/PlayerAttackCollision.h"
#include "../Collision/Field/FieldCollision.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
    // Ninjaの移動前の位置を保存
    VECTOR oldNinjaPos =
        m_ninja.GetPosition();

    m_ninja.Update();

    // Wallとの当たり判定
    if (FieldCollision::CheckWall(
        m_ninja,
        m_field))
    {
        // 壁に入ったら移動前の位置に戻す
        m_ninja.SetPosition(
            oldNinjaPos);
    }

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

    // Enemy攻撃とPlayerの当たり判定
    if (!m_enemy.IsDead() &&
        m_enemy.IsAttackActive() &&
        !m_ninja.IsKnockback())
    {
        VECTOR start =
            m_enemy.GetAttackStartPos();

        VECTOR target =
            m_enemy.GetAttackTargetPos();

        if (EnemyAttackCollision::Check(
            m_ninja,
            start,
            target))
        {
            // EnemyからPlayerへ向かう方向
            VECTOR playerPos =
                m_ninja.GetPosition();

            float dx =
                playerPos.x -
                start.x;

            float dz =
                playerPos.z -
                start.z;

            float length =
                sqrtf(
                    dx * dx +
                    dz * dz);

            if (length > 0.001f)
            {
                dx /= length;
                dz /= length;

                VECTOR knockbackDirection =
                    VGet(
                        dx,
                        0.0f,
                        dz);

                m_ninja.ApplyKnockback(
                    knockbackDirection,
                    0.5f,
                    0.5f);
            }
        }
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
    VECTOR playerPos =
        m_ninja.GetPosition();

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
        VECTOR playerPos =
            m_ninja.GetPosition();

        m_drawManager.Add(
            playerPos,
            playerPos.z,
            [&]()
            {
                m_playerAttack.DrawShoot();
            });
    }

    // 敵登録
    VECTOR enemyPos =
        m_enemy.GetPosition();

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

    // Enemy攻撃判定デバッグ
    if (!m_enemy.IsDead() &&
        m_enemy.IsAttackActive())
    {
        VECTOR start =
            m_enemy.GetAttackStartPos();

        VECTOR target =
            m_enemy.GetAttackTargetPos();

        EnemyAttackCollision::DrawDebug(
            start,
            target);
    }

    // Wallの当たり判定デバッグ
    FieldCollision::DrawDebug(
        m_field);
}