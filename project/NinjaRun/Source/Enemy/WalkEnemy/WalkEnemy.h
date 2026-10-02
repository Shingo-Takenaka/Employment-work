#pragma once

#include "DxLib.h"

#include "../EnemyBase/EnemyBase.h"
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
	// 移動速度
	float m_moveSpeed;

	// 移動範囲
	float m_minX;
	float m_maxX;

	// 移動方向 trueなら左、falseなら右
	bool m_isMoveLeft;

	// アニメーション
	WalkEnemyAnimation m_animation;

};