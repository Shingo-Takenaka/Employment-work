#include "PlayScene.h"
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
    VECTOR oldNinjaPos = m_ninja.GetPosition();

    m_ninja.Update();

    // ダッシュ切りの当たり判定を更新
    m_dashAttackCollision.Update(m_ninja);

    // Wallとの当たり判定
    // 壁との当たり判定
    if (m_fieldCollision.CheckWall(m_ninja, m_field))
    {
        m_fieldCollision.ResolveWallCollision(m_ninja,m_field,oldNinjaPos);
    }

    // 敵を取得
    auto& normalEnemies = m_enemyManager.GetNormalEnemies();
    auto& walkEnemies = m_enemyManager.GetWalkEnemies();

    // NormalEnemyへの通常攻撃とダッシュ切り
    for (auto& enemy : normalEnemies)
    {
        enemy.CheckPlayerAttack(
            m_ninja,
            m_playerAttackCollision,
            m_dashAttackCollision);
    }

    // WalkEnemyへの通常攻撃とダッシュ切り
    for (auto& enemy : walkEnemies)
    {
        enemy.CheckPlayerAttack(
            m_ninja,
            m_playerAttackCollision,
            m_dashAttackCollision);
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

        VECTOR start = enemy.GetAttackStartPos();
        VECTOR target = enemy.GetAttackTargetPos();

        if (EnemyAttackCollision::Check(m_ninja, start, target))
        {
            VECTOR playerPos = m_ninja.GetPosition();

            float dx = playerPos.x - start.x;
            float dz = playerPos.z - start.z;

            float length = sqrtf(dx * dx + dz * dz);

            if (length > 0.001f)
            {
                dx /= length;
                dz /= length;

                VECTOR knockbackDirection = VGet(dx, 0.0f, dz);

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

        if (TouchEnemy::Check(m_ninja, enemy))
        {
            if (!m_ninja.IsKnockback())
            {
                TouchEnemy::Apply(m_ninja, enemy);
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

        if (TouchEnemy::Check(m_ninja, enemy))
        {
            if (!m_ninja.IsKnockback())
            {
                TouchEnemy::Apply(m_ninja, enemy);
            }
        }
    }

    // カメラ更新
    m_camera.Update(m_ninja.GetPosition());

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
    for (int i = m_playerAttack.GetShootCount() - 1; i >= 0; i--)
    {
        bool isHit = false;

        // NormalEnemyとの当たり判定
        for (auto& enemy : normalEnemies)
        {
            if (enemy.IsDead())
            {
                continue;
            }

            if (ShootCollision::Check(m_playerAttack, i, enemy))
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

                if (ShootCollision::Check(m_playerAttack, i, enemy))
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

    // Player
    VECTOR playerPos = m_ninja.GetPosition();
    m_drawManager.Add(
        playerPos,
        playerPos.z,
        [&]()
        {
            m_ninja.Draw();
        });

    // 手裏剣の描画
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

    // NormalEnemy
    auto& normalEnemies = m_enemyManager.GetNormalEnemies();

    // NormalEnemyの描画処理を追加
    for (auto& enemy : normalEnemies)
    {
        if (enemy.IsDead())
        {
            continue;
        }

        VECTOR enemyPos = enemy.GetPosition();
        NormalEnemy* enemyPtr = &enemy;

        m_drawManager.Add(
            enemyPos,
            enemyPos.z,
            [enemyPtr]()
            {
                enemyPtr->Draw();
            });
    }

    // WalkEnemy
    auto& walkEnemies = m_enemyManager.GetWalkEnemies();

    // WalkEnemyの描画処理を追加
    for (auto& enemy : walkEnemies)
    {
        if (enemy.IsDead())
        {
            continue;
        }

        VECTOR enemyPos = enemy.GetPosition();
        WalkEnemy* enemyPtr = &enemy;

        m_drawManager.Add
        (
            enemyPos,enemyPos.z,[enemyPtr]()
            {
                enemyPtr->Draw();
            }
        );
    }

    // ソートして描画
    m_drawManager.Draw();

    // =====デバッグ表示=====
    // Ctrl+K → Ctrl+C で選択した部分のコメントアウト
    // Ctrl+K → Ctrl+U で選択した部分のコメントアウト解除
    
    //// Playerの接触判定範囲
    //CollisionDebug::DrawPlayerBody(playerPos);

    //// Playerの通常攻撃判定範囲
    //if (m_ninja.IsSlash())
    //{
    //    CollisionDebug::DrawPlayerAttack(
    //        playerPos,
    //        m_ninja.IsReverseX());
    //}

    //// 手裏剣の当たり判定範囲
    //for (int i = 0; i < m_playerAttack.GetShootCount(); i++)
    //{
    //    VECTOR shootPos = m_playerAttack.GetShootPosition(i);

    //    CollisionDebug::DrawShuriken(shootPos);
    //}

    //// NormalEnemyの接触判定範囲と攻撃判定範囲
    //for (auto& enemy : normalEnemies)
    //{
    //    if (enemy.IsDead())
    //    {
    //        continue;
    //    }

    //    VECTOR enemyPos = enemy.GetPosition();

    //    // 敵の接触判定範囲
    //    CollisionDebug::DrawEnemyBody(enemyPos);

    //    // 敵の攻撃判定範囲
    //    if (enemy.IsAttackActive())
    //    {
    //        VECTOR start = enemy.GetAttackStartPos();
    //        VECTOR target = enemy.GetAttackTargetPos();

    //        CollisionDebug::DrawEnemyAttack(start, target);
    //    }
    //}

    //// WalkEnemyの接触判定範囲
    //for (auto& enemy : walkEnemies)
    //{
    //    if (enemy.IsDead())
    //   {
    //        continue;
    //    }

    //    VECTOR enemyPos = enemy.GetPosition();

    //   CollisionDebug::DrawEnemyBody(enemyPos);
    //}
    // Playerの座標
    DrawFormatString(
        10, 10,
        GetColor(255, 255, 255),
        "Player X: %.2f Y: %.2f Z: %.2f",
        playerPos.x,
        playerPos.y,
        playerPos.z);
    //==========
}