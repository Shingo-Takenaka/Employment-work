#pragma once

#include "DxLib.h"

#include"../EnemyBase/EnemyBase.h"
#include "WalkEnemyAnimation.h"

class Ninja;

class WalkEnemy : public EnemyBase
{
public:

	WalkEnemy();
	~WalkEnemy();

	void Update(const Ninja& ninja) override;
	void Draw() override;

private:

	// Player座標
	VECTOR m_playerPos;

	// アニメーション
	WalkEnemyAnimation m_animation;
};