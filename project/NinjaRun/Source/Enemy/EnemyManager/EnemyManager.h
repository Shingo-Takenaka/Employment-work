#pragma once

#include "DxLib.h"

#include <vector>

#include "../NormalEnemy/NormalEnemy.h"
#include "../WalkEnemy/WalkEnemy.h"

class Ninja;

class EnemyManager
{
public:

    EnemyManager();
    ~EnemyManager();

    // “G‚ğ’Ç‰Á
    void AddNormalEnemy(VECTOR position);
    void AddWalkEnemy(VECTOR position);

    // “G‚ÌXV
    void Update(const Ninja& ninja);

    // NormalEnemy‚ğæ“¾
    std::vector<NormalEnemy>& GetNormalEnemies();

    // WalkEnemy‚ğæ“¾
    std::vector<WalkEnemy>& GetWalkEnemies();

private:

    // NormalEnemy
    std::vector<NormalEnemy> m_normalEnemies;

    // WalkEnemy
    std::vector<WalkEnemy> m_walkEnemies;
};