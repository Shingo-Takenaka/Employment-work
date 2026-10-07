#include "PlayScene.h"

#include "../Collision/TouchEnemyDebug.h"
#include "../Collision/PlayerAttackCollision.h"
#include "../Collision/Field/FieldCollision.h"

#include <math.h>

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
        m_ninja.SetPosition(oldNinjaPos);

        // Ninjaが空中にいる場合
        if (m_ninja.IsJump())
        {
            VECTOR ninjaPos =
                m_ninja.GetPosition();

            VECTOR wallPos =
                m_field.GetWallPosition();

            VECTOR wallKickDirection =
                VGet(0.0f, 0.0f, 0.0f);

            // 壁がNinjaの右側にある場合
            if (wallPos.x > ninjaPos.x)
            {
                wallKickDirection.x = -1.0f;
            }
            // 壁がNinjaの左側にある場合
            else
            {
                wallKickDirection.x = 1.0f;
            }

            // 壁キック可能状態にする
            m_ninja.EnableWallKick(
                wallKickDirection);
        }
    }

    // 敵一覧を取得
    auto& normalEnemies =
        m_enemyManager.GetNormalEnemies();

    auto& walkEnemies =
        m_enemyManager.GetWalkEnemies();

    // Playerの近接攻撃とNormalEnemyの当たり判定
    for (auto& enemy : normalEnemies)
    {
        if (!enemy.IsDead() &&
            m_playerAttackCollision.Check(
                m_ninja,
                enemy))
        {
            enemy.TakeDamage(1);
        }
    }

    // Playerの近接攻撃とWalkEnemyの当たり判定
    for (auto& enemy : walkEnemies)
    {
        if (!enemy.IsDead() &&
            m_playerAttackCollision.Check(
                m_ninja,
                enemy))
        {
            enemy.TakeDamage(1);
        }
    }

    // 敵更新
    m_enemyManager.Update(m_ninja);

    // NormalEnemy攻撃とPlayerの当たり判定
    for (auto& enemy : normalEnemies)
    {
        if (enemy.IsDead() ||
            !enemy.IsAttackActive() ||
            m_ninja.IsKnockback())
        {
            continue;
        }

        VECTOR start =
            enemy.GetAttackStartPos();

        VECTOR target =
            enemy.GetAttackTargetPos();

        if (EnemyAttackCollision::Check(
            m_ninja,
            start,
            target))
        {
            VECTOR playerPos =
                m_ninja.GetPosition();

            float dx =
                playerPos.x - start.x;

            float dz =
                playerPos.z - start.z;

            float length =
                sqrtf(dx * dx + dz * dz);

            if (length > 0.001f)
            {
                dx /= length;
                dz /= length;

                VECTOR knockbackDirection =
                    VGet(dx, 0.0f, dz);

                m_ninja.ApplyKnockback(
                    knockbackDirection,
                    0.5f,
                    0.5f);
            }
        }
    }

    // NormalEnemyとの接触
    for (auto& enemy : normalEnemies)
    {
        if (enemy.IsDead())
        {
            continue;
        }

        if (TouchEnemy::Check(
            m_ninja,
            enemy))
        {
            if (!m_ninja.IsKnockback())
            {
                TouchEnemy::Apply(
                    m_ninja,
                    enemy);
            }
        }
    }

    // WalkEnemyとの接触
    for (auto& enemy : walkEnemies)
    {
        if (enemy.IsDead())
        {
            continue;
        }

        if (TouchEnemy::Check(
            m_ninja,
            enemy))
        {
            if (!m_ninja.IsKnockback())
            {
                TouchEnemy::Apply(
                    m_ninja,
                    enemy);
            }
        }
    }

    // カメラ更新
    m_camera.Update(
        m_ninja.GetPosition());

    // 手裏剣アニメーション開始時に発射
    if (m_ninja.IsShootStart())
    {
        m_playerAttack.CreateShoot(
            m_ninja.GetPosition(),
            m_ninja.IsReverseX());
    }

    // 手裏剣更新
    m_playerAttack.UpdateShoot();

    // 手裏剣と敵の当たり判定
    for (int i = m_playerAttack.GetShootCount() - 1;
        i >= 0;
        i--)
    {
        bool isHit = false;

        // NormalEnemyとの当たり判定
        for (auto& enemy : normalEnemies)
        {
            if (enemy.IsDead())
            {
                continue;
            }

            if (ShootCollision::Check(
                m_playerAttack,
                i,
                enemy))
            {
                enemy.TakeDamage(1);
                isHit = true;
                break;
            }
        }

        // WalkEnemyとの当たり判定
        if (!isHit)
        {
            for (auto& enemy : walkEnemies)
            {
                if (enemy.IsDead())
                {
                    continue;
                }

                if (ShootCollision::Check(
                    m_playerAttack,
                    i,
                    enemy))
                {
                    enemy.TakeDamage(1);
                    isHit = true;
                    break;
                }
            }
        }

        // 当たった手裏剣を削除
        if (isHit)
        {
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

    // Player登録
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
        m_drawManager.Add(
            playerPos,
            playerPos.z,
            [&]()
            {
                m_playerAttack.DrawShoot();
            });
    }

    // 敵を描画
    // NormalEnemy登録
    auto& normalEnemies =
        m_enemyManager.GetNormalEnemies();

    for (auto& enemy : normalEnemies)
    {
        VECTOR enemyPos =
            enemy.GetPosition();

        NormalEnemy* enemyPtr = &enemy;

        m_drawManager.Add(
            enemyPos,
            enemyPos.z,
            [enemyPtr]()
            {
                enemyPtr->Draw();
            });
    }

    // WalkEnemy登録
    auto& walkEnemies =
        m_enemyManager.GetWalkEnemies();

    for (auto& enemy : walkEnemies)
    {
        VECTOR enemyPos =
            enemy.GetPosition();

        m_drawManager.Add(
            enemyPos,
            enemyPos.z,
            [&enemy]()
            {
                enemy.Draw();
            });
    }

    // ソートして描画
    m_drawManager.Draw();

    // カメラデバッグ
    m_camera.Draw();

    // NormalEnemy接触判定デバッグ
    /*
    for (auto& enemy : normalEnemies)
    {
        if (!enemy.IsDead())
        {
            TouchEnemyDebug::Draw(
                m_ninja,
                enemy);
        }
    }
    */

    // WalkEnemy接触判定デバッグ
    /*
    for (auto& enemy : walkEnemies)
    {
        if (!enemy.IsDead())
        {
            TouchEnemyDebug::Draw(
                m_ninja,
                enemy);
        }
    }
    */

    // NormalEnemy攻撃判定デバッグ
    for (auto& enemy : normalEnemies)
    {
        if (!enemy.IsDead() &&
            enemy.IsAttackActive())
        {
            VECTOR start =
                enemy.GetAttackStartPos();

            VECTOR target =
                enemy.GetAttackTargetPos();

            EnemyAttackCollision::DrawDebug(
                start,
                target);
        }
    }

    // Wallの当たり判定デバッグ
    /*
    FieldCollision::DrawDebug(
        m_field);
    */

    // Playerの座標
    DrawFormatString(
        10, 10,
        GetColor(255, 255, 255),
        "Player X: %.2f Y: %.2f Z: %.2f",
        playerPos.x,
        playerPos.y,
        playerPos.z);
}