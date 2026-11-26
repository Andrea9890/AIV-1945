#include "gfx/animation.h"
#include <math.h>

static float TotalDuration(const Animation* animation)
{
    return animation->count * animation->frameTime;
}

Animation AnimationInit(SpriteId first, int count, float frameTime, bool loop)
{
    return (Animation){ .first = first, .count = count, .frameTime = frameTime, .loop = loop };
}

void AnimationUpdate(Animation* animation, float deltaTime)
{
    if (animation->count <= 0 || animation->frameTime <= 0.0f) return;

    float total = TotalDuration(animation);
    animation->timer += deltaTime;

    if (animation->loop)
    {
        animation->timer = fmodf(animation->timer, total);
    }
    else if (animation->timer > total)
    {
        animation->timer = total;
    }
}

SpriteId AnimationCurrentFrame(const Animation* animation)
{
    if (animation->count <= 0 || animation->frameTime <= 0.0f) return animation->first;

    int index = (int)(animation->timer / animation->frameTime);

    if (index > animation->count - 1)
    {
        index = animation->count - 1;
    }

    return (SpriteId)(animation->first + index);
}

bool AnimationIsFinished(const Animation* animation)
{
    if (animation->loop) return false;
    if (animation->count <= 0 || animation->frameTime <= 0.0f) return true;

    return animation->timer >= TotalDuration(animation);
}

void AnimationReset(Animation* animation)
{
    animation->timer = 0.0f;
}
