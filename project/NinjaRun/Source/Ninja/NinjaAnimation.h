#pragma once

#include "DxLib.h"
#include "../Animation/Animation.h"

enum class NinjaAnim
{
    WAIT,
    WALK,
    JUMP,
    FALLING,
    SLASH,
    GUARD,
    SHOOT,
    DASH,
    DASH_SLASH,
    MAX
};

struct SpriteAnimation
{
    static const int MAX_FRAME = 16;

    int graph[MAX_FRAME];
    int frameNum;
    Animation anim;
};

class NinjaAnimation
{
public:
    NinjaAnimation();

    bool LoadAnimations();

    void Play(NinjaAnim animation);

    void Update(
        bool isMove,
        bool isJump,
        float jumpSpeed,
        bool& isSlash,
        bool isGuard,
        bool& isShoot,
        bool isDash,
        bool isDashCut);

    void Draw(
        VECTOR pos,
        float size,
        bool isReverseX);

private:
    bool LoadAnimation(
        SpriteAnimation& animation,
        const char* fileName,
        int frameNum,
        int width,
        int height,
        int interval);

    void ChangeAnimation(NinjaAnim animation);

    SpriteAnimation m_animation[(int)NinjaAnim::MAX];

    NinjaAnim m_currentAnim;
};