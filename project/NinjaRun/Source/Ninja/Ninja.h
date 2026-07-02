#pragma once

#include "DxLib.h"
#include "../Animation/Animation.h"

// アニメーションの種類
enum class NinjaAnim
{
    WAIT,   // 待機
    WALK,   // 歩き

    MAX
};

// アニメーション情報
struct SpriteAnimation
{
	//アニメーションに使える最大フレーム数
    static const int MAX_FRAME = 16;

    int graph[MAX_FRAME];

    int frameNum;

    Animation anim;
};

class Ninja
{
public:

    Ninja();
    ~Ninja();

    void Update();
    void Draw();

    VECTOR GetPosition() const;

private:
    
    VECTOR m_pos;

    float m_size;

    float m_moveSpeed;

    // 向き
    bool m_isReverseX;

    // アニメーション
    SpriteAnimation m_animation[(int)NinjaAnim::MAX];

    NinjaAnim m_currentAnim;

private:

    bool LoadAnimation(
        SpriteAnimation& animation,
        const char* fileName,
		int frameNum,   // フレーム数
		int width,      // 1フレームの幅
		int height,     // 1フレームの高さ
		int interval);  // フレーム切り替え間隔
};