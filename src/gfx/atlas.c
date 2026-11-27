#include "gfx/atlas.h"
#include <math.h>

static const char* const ATLAS_FILE     = "assets/1945_atlas.png";

static constexpr int ATLAS_GRID_START   = 4;
static constexpr int ATLAS_CELL_SIZE    = 32;
static constexpr int ATLAS_LINE_SIZE    = 1;
static constexpr int ATLAS_CELL_MARGIN  = 1;

#define ATLAS_CELL(x, y, w, h) {(x) + ATLAS_CELL_MARGIN, (y) + ATLAS_CELL_MARGIN, (w) - 2 * ATLAS_CELL_MARGIN, (h) - 2 * ATLAS_CELL_MARGIN}

#define ATLAS_GRID(col, row, cols, rows) ATLAS_CELL(ATLAS_GRID_START + (col) * (ATLAS_CELL_SIZE + ATLAS_LINE_SIZE), ATLAS_GRID_START + (row) * (ATLAS_CELL_SIZE + ATLAS_LINE_SIZE), (cols) * ATLAS_CELL_SIZE + ((cols) - 1) * ATLAS_LINE_SIZE, (rows) * ATLAS_CELL_SIZE + ((rows) - 1) * ATLAS_LINE_SIZE)

static const Rectangle SPRITE_FRAMES[] =
{
    [SPRITE_PLAYER_0]           = ATLAS_GRID(0, 12, 2, 2),
    [SPRITE_PLAYER_1]           = ATLAS_GRID(2, 12, 2, 2),
    [SPRITE_PLAYER_2]           = ATLAS_GRID(4, 12, 2, 2),

    [SPRITE_ENEMY_GREEN_0]      = ATLAS_GRID(6, 14, 1, 1),
    [SPRITE_ENEMY_GREEN_1]      = ATLAS_GRID(7, 14, 1, 1),
    [SPRITE_ENEMY_GREEN_2]      = ATLAS_GRID(8, 14, 1, 1),

    [SPRITE_ENEMY_WHITE_0]      = ATLAS_GRID(3, 14, 1, 1),
    [SPRITE_ENEMY_WHITE_1]      = ATLAS_GRID(4, 14, 1, 1),
    [SPRITE_ENEMY_WHITE_2]      = ATLAS_GRID(5, 14, 1, 1),

    [SPRITE_ENEMY_BIG_0]        = ATLAS_GRID(8, 12, 2, 2),
    [SPRITE_ENEMY_BIG_1]        = ATLAS_GRID(10, 12, 2, 2),
    [SPRITE_ENEMY_BIG_2]        = ATLAS_GRID(12, 12, 2, 2),

    [SPRITE_PLAYER_BULLET]      = ATLAS_GRID(0, 5, 1, 1),
    [SPRITE_ENEMY_BULLET]       = ATLAS_GRID(1, 6, 1, 1),

    [SPRITE_EXPLOSION_SMALL_0]  = ATLAS_GRID(2, 5, 1, 1),
    [SPRITE_EXPLOSION_SMALL_1]  = ATLAS_GRID(3, 5, 1, 1),
    [SPRITE_EXPLOSION_SMALL_2]  = ATLAS_GRID(4, 5, 1, 1),
    [SPRITE_EXPLOSION_SMALL_3]  = ATLAS_GRID(5, 5, 1, 1),
    [SPRITE_EXPLOSION_SMALL_4]  = ATLAS_GRID(6, 5, 1, 1),
    [SPRITE_EXPLOSION_SMALL_5]  = ATLAS_GRID(7, 5, 1, 1),

    [SPRITE_EXPLOSION_BIG_0]    = ATLAS_GRID(0, 9, 2, 2),
    [SPRITE_EXPLOSION_BIG_1]    = ATLAS_GRID(2, 9, 2, 2),
    [SPRITE_EXPLOSION_BIG_2]    = ATLAS_GRID(4, 9, 2, 2),
    [SPRITE_EXPLOSION_BIG_3]    = ATLAS_GRID(6, 9, 2, 2),
    [SPRITE_EXPLOSION_BIG_4]    = ATLAS_GRID(8, 9, 2, 2),
    [SPRITE_EXPLOSION_BIG_5]    = ATLAS_GRID(10, 9, 2, 2),
    [SPRITE_EXPLOSION_BIG_6]    = ATLAS_GRID(12, 9, 2, 2),

    [SPRITE_ISLAND_0]           = ATLAS_CELL(103, 499, 64, 65),
    [SPRITE_ISLAND_1]           = ATLAS_CELL(168, 499, 64, 65),
    [SPRITE_ISLAND_2]           = ATLAS_CELL(233, 499, 64, 65),

    [SPRITE_SEA]                = ATLAS_GRID(8, 11, 1, 1),
};

bool AtlasLoad(Atlas* atlas)
{
    *atlas = (Atlas){};

    const char* path = TextFormat("%s%s", GetApplicationDirectory(), ATLAS_FILE);
    atlas->texture = LoadTexture(path);

    if (!IsTextureValid(atlas->texture))
    {
        TraceLog(LOG_ERROR, "ATLAS: could not load '%s'", path);
        return false;
    }

    for (int i = 0; i < SPRITE_COUNT; i++)
    {
        atlas->frames[i] = SPRITE_FRAMES[i];

        if (atlas->frames[i].width <= 0 || atlas->frames[i].height <= 0)
        {
            TraceLog(LOG_WARNING, "ATLAS: sprite %d has no rectangle in SPRITE_FRAMES", i);
        }
    }

    return true;
}

void AtlasUnload(Atlas* atlas)
{
    if (atlas == nullptr) return;

    if (IsTextureValid(atlas->texture))
    {
        UnloadTexture(atlas->texture);
    }

    *atlas = (Atlas){};
}

void AtlasDraw(const Atlas* atlas, SpriteId id, Vector2 center, Color tint)
{
    Rectangle frame = atlas->frames[id];

    Vector2 topLeft =
    {
        floorf(center.x - frame.width * 0.5f),
        floorf(center.y - frame.height * 0.5f),
    };

    DrawTextureRec(atlas->texture, frame, topLeft, tint);
}

void AtlasDrawRotated(const Atlas* atlas, SpriteId id, Vector2 center, float rotation, Color tint)
{
    Rectangle frame = atlas->frames[id];
    Rectangle dest = { center.x, center.y, frame.width, frame.height };
    Vector2 origin = { frame.width * 0.5f, frame.height * 0.5f };

    DrawTexturePro(atlas->texture, frame, dest, origin, rotation, tint);
}
