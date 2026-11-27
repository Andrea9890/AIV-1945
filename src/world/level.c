#include "world/level.h"
#include <math.h>

static SpriteId RandomIslandSprite()
{
    return (SpriteId)GetRandomValue(SPRITE_ISLAND_0, SPRITE_ISLAND_2);
}

static bool OverlapsOthers(const Level* level, int index, Vector2 pos)
{
    for (int i = 0; i < LEVEL_MAX_ISLANDS; i++)
    {
        if (i == index || !level->islands[i].active) continue;

        float dx = level->islands[i].pos.x - pos.x;
        float dy = level->islands[i].pos.y - pos.y;

        if (dx * dx + dy * dy < 4.0f * LEVEL_ISLAND_RADIUS * LEVEL_ISLAND_RADIUS)
        {
            return true;
        }
    }
    return false;
}

static void PlaceIsland(Level* level, int index, int minY, int maxY)
{
    Vector2 pos = {};

    for (int attempt = 0; attempt < LEVEL_PLACE_ATTEMPTS; attempt++)
    {
        pos.x = (float)GetRandomValue(LEVEL_ISLAND_RADIUS, SCREEN_WIDTH - LEVEL_ISLAND_RADIUS);
        pos.y = (float)GetRandomValue(minY, maxY);

        if (!OverlapsOthers(level, index, pos)) break;
    }

    level->islands[index] = (Island){ .sprite = RandomIslandSprite(), .pos = pos, .rotation = (float)GetRandomValue(0, 359), .active = true };
}

void LevelInit(Level* level)
{
    *level = (Level){ .speed = LEVEL_SCROLL_SPEED };

    for (int i = 0; i < LEVEL_MAX_ISLANDS; i++)
    {
        PlaceIsland(level, i, 0, SCREEN_HEIGHT);
    }
}

void LevelUpdate(Level* level, float deltaTime)
{
    float distance = level->speed * deltaTime;

    level->scroll = fmodf(level->scroll + distance, (float)LEVEL_SEA_TILE_SIZE);

    for (int i = 0; i < LEVEL_MAX_ISLANDS; i++)
    {
        Island* island = &level->islands[i];
        if (!island->active) continue;

        island->pos.y += distance;

        if (island->pos.y - LEVEL_ISLAND_RADIUS > SCREEN_HEIGHT)
        {
            int top = -LEVEL_ISLAND_RADIUS;
            PlaceIsland(level, i, top - LEVEL_RESPAWN_SPAN, top - 1);
        }
    }
}

static void DrawSea(const Level* level, const Atlas* atlas)
{
    Rectangle tile = atlas->frames[SPRITE_SEA];

    float firstY = level->scroll - LEVEL_SEA_TILE_SIZE;

    for (int row = 0; firstY + row * LEVEL_SEA_TILE_SIZE < SCREEN_HEIGHT; row++)
    {
        float y = floorf(firstY + row * LEVEL_SEA_TILE_SIZE);

        for (int x = 0; x < SCREEN_WIDTH; x += LEVEL_SEA_TILE_SIZE)
        {
            DrawTextureRec(atlas->texture, tile, (Vector2){ (float)x, y }, WHITE);
        }
    }
}

void LevelDraw(const Level* level, const Atlas* atlas)
{
    DrawSea(level, atlas);

    for (int i = 0; i < LEVEL_MAX_ISLANDS; i++)
    {
        if (level->islands[i].active)
        {
            AtlasDrawRotated(atlas, level->islands[i].sprite, level->islands[i].pos, level->islands[i].rotation, WHITE);
        }
    }
}
