#pragma once

#include "gfx/atlas.h"

typedef struct Animation
{
    SpriteId first;
    int count;
    float frameTime;
    float timer;
    bool loop;
} Animation;

Animation AnimationInit(SpriteId first, int count, float frameTime, bool loop);
void AnimationUpdate(Animation* animation, float deltaTime);
SpriteId AnimationCurrentFrame(const Animation* animation);
bool AnimationIsFinished(const Animation* animation);
void AnimationReset(Animation* animation);
