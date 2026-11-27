#pragma once

#include "config.h"
#include "gfx/atlas.h"
#include "raylib/raylib.h"

typedef struct Island
{
    SpriteId sprite;
    Vector2 pos;
    float rotation;
    bool active;
} Island;

typedef struct Level
{
    float scroll;
    float speed;
    Island islands[LEVEL_MAX_ISLANDS];
} Level;

void LevelInit(Level* level);
void LevelUpdate(Level* level, float deltaTime);
void LevelDraw(const Level* level, const Atlas* atlas);
