#pragma once

#include "DxLib.h"

#include "../../Animation/Animation.h"

enum class WalkEnemyAnim
{
	WAIT,
	WALK,

	MAX
};

struct WalkEnemySpriteAnimation
{
	static const int MAX_FRAME = 16;

	int graph[MAX_FRAME];

	int frameNum;

	Animation anim;
};

class WalkEnemyAnimation
{
public:

	WalkEnemyAnimation();
	~WalkEnemyAnimation();

	void LoadAnimaiton();

	void Update();

	void SetAnimation(WalkEnemyAnim anim);

	WalkEnemyAnim GetCurrentAnimation() const;

	int GetFreme() const;

	void Reset();

	void DrawAnimation(
		VECTOR pos, float size, bool isReverseX);

private:

	bool LoadAnimation(
		WalkEnemySpriteAnimation& animation,
		const char* fileName,
		int fremeNum,
		int width,
		int height,
		int interval);

private:

	WalkEnemySpriteAnimation m_animation[(int)WalkEnemyAnim::MAX];

	WalkEnemyAnim m_currentAnim;
};