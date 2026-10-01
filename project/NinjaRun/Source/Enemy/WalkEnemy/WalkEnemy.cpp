#include "WalkEnemy.h"
#include "../../Ninja/Ninja.h"

#include <math.h>

WalkEnemy::WalkEnemy()
	: EnemyBase(1)
{
	m_pos = VGet(50.0f, 0.0f, 50.0f);

	m_isReverseX = false;

	m_animation.LoadAnimaiton();
}

WalkEnemy::~WalkEnemy()
{
}

void WalkEnemy::Update(const Ninja& ninja)
{
	if (m_isDead)
	{
		return;
	}

	m_playerPos = ninja.GetPosition();

	VECTOR ninjaPos = ninja.GetPosition();

	float dx = ninjaPos.x - m_pos.x;

	float dz = ninjaPos.z - m_pos.z;

	float distance = sqrtf(dx * dx + dz * dz);



	m_animation.Update();
}