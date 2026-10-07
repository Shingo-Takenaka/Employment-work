#include "EnemyManager.h"

#include "../../Ninja/Ninja.h"

EnemyManager::EnemyManager()
{
    // NormalEnemy
    m_normalEnemies.reserve(10);
    // WalkEmemy
    m_walkEnemies.reserve(10);

    // NormalEnemyPos
    AddNormalEnemy(VGet(40.0f, 0.0f, 50.0f));
    AddNormalEnemy(VGet(80.0f, 0.0f, 50.0f));

    // WalkEnemyPos
    AddWalkEnemy(VGet(50.0f, 0.0f, 100.0f));
    AddWalkEnemy(VGet(90.0f, 0.0f, 100.0f));
}

EnemyManager::~EnemyManager()
{
}

// NormalEnemy‚ğ’Ç‰Á
void EnemyManager::AddNormalEnemy(VECTOR position)
{
    m_normalEnemies.emplace_back();

    m_normalEnemies.back().SetPosition(position);
}

// WalkEnemy‚ğ’Ç‰Á
void EnemyManager::AddWalkEnemy(VECTOR position)
{
    m_walkEnemies.emplace_back();

    m_walkEnemies.back().SetPosition(position);
}

// “G‚ÌXV
void EnemyManager::Update(const Ninja& ninja)
{
    for (auto& enemy : m_normalEnemies)
    {
        if (!enemy.IsDead())
        {
            enemy.Update(ninja);
        }
    }

    for (auto& enemy : m_walkEnemies)
    {
        if (!enemy.IsDead())
        {
            enemy.Update(ninja);
        }
    }
}

// NormalEnemy‚ğæ“¾
std::vector<NormalEnemy>& EnemyManager::GetNormalEnemies()
{
    return m_normalEnemies;
}

// WalkEnemy‚ğæ“¾
std::vector<WalkEnemy>& EnemyManager::GetWalkEnemies()
{
    return m_walkEnemies;
}