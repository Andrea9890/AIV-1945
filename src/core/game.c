#include "core/game.h"
#include "config.h"
#include "raylib/raylib.h"

static const char* const STATE_NAMES[] = {
    [STATE_SPLASH]      = "SPLASH",
    [STATE_MENU]        = "MENU",
    [STATE_PLAYING]     = "PLAYING",
    [STATE_GAME_OVER]   = "GAME OVER",
    [STATE_TEST]        = "TEST",
};

static constexpr int STATE_NAME_FONT_SIZE   = 40;

static constexpr int TEST_COLUMNS           = 8;
static constexpr int TEST_SLOT_WIDTH        = 80;
static constexpr int TEST_SLOT_HEIGHT       = 100;
static constexpr int TEST_TOP               = 40;
static constexpr int TEST_FONT_SIZE         = 20;
static constexpr int TEST_CHECKER_SIZE      = 8;

static void LoadWindowIcon()
{
    const char* path = TextFormat("%s%s", GetApplicationDirectory(), "assets/icon.png");
    Image icon = LoadImage(path);

    if (!IsImageValid(icon))
    {
        TraceLog(LOG_WARNING, "GAME: could not load window icon '%s'", path);
        return;
    }

    SetWindowIcon(icon);
    UnloadImage(icon);
}

bool GameInit(Game* game)
{
    *game = (Game){ .state = STATE_SPLASH };

    LoadWindowIcon();

    if (!AtlasLoad(&game->atlas))
    {
        return false;
    }

    return true;
}

void GameDeinit(Game* game)
{
    if (game == nullptr) return;

    AtlasUnload(&game->atlas);

    *game = (Game){};
}

void GameChangeState(Game* game, GameState next)
{
    switch (next)
    {
        case STATE_MENU:
            game->menuIndex = 0;
            break;

        case STATE_PLAYING:
            game->score = 0;
            break;

        case STATE_GAME_OVER:
            if (game->score > game->highScore)
            {
                game->highScore = game->score;
            }
            break;

        case STATE_SPLASH:
        case STATE_TEST:
        case STATE_COUNT:
            break;
    }

    game->state = next;
}

bool GameShouldQuit(const Game *game)
{
    return game->quit;
}

#pragma region Update

static void AdvanceOnEnter(Game *game, GameState next)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        GameChangeState(game, next);
    }
}

static void UpdateSplash(Game* game, float deltaTime)
{
    AdvanceOnEnter(game, STATE_MENU);
}

static void UpdateMenu(Game* game, float deltaTime)
{
    AdvanceOnEnter(game, STATE_PLAYING);
}

static void UpdatePlaying(Game* game, float deltaTime)
{
    AdvanceOnEnter(game, STATE_GAME_OVER);
}

static void UpdateGameOver(Game* game, float deltaTime)
{
    AdvanceOnEnter(game, STATE_MENU);
}

static void ToggleTest(Game* game)
{
    if (game->state == STATE_TEST)
    {
        game->state = game->stateBeforeTest;
    }
    else
    {
        game->stateBeforeTest = game->state;
        game->state = STATE_TEST;
    }
}

void GameUpdate(Game* game, float deltaTime)
{
    if (IsKeyPressed(KEY_F1))
    {
        ToggleTest(game);
        return;
    }

    switch (game->state)
    {
        case STATE_SPLASH:
            UpdateSplash(game, deltaTime);
            break;

        case STATE_MENU:
            UpdateMenu(game, deltaTime);
            break;

        case STATE_PLAYING:
            UpdatePlaying(game, deltaTime);
            break;

        case STATE_GAME_OVER:
            UpdateGameOver(game, deltaTime);
            break;

        case STATE_TEST:
        case STATE_COUNT: break;
    }
}

#pragma endregion

#pragma region Draw

static void DrawStateName(GameState state)
{
    const char *name = STATE_NAMES[state];
    int width = MeasureText(name, STATE_NAME_FONT_SIZE);
    DrawText(name,(SCREEN_WIDTH - width) * 0.5f,(SCREEN_HEIGHT - STATE_NAME_FONT_SIZE) * 0.5f, STATE_NAME_FONT_SIZE, RAYWHITE);
}

static void DrawSplash(const Game* game)
{
    DrawText("TEST ATLAS - F1 to open", 8, 8, TEST_FONT_SIZE, RAYWHITE);
    DrawStateName(STATE_SPLASH);
}

static void DrawMenu(const Game* game)
{
    DrawText("TEST ATLAS - F1 to open", 8, 8, TEST_FONT_SIZE, RAYWHITE);
    DrawStateName(STATE_MENU);
}

static void DrawPlaying(const Game* game)
{
    DrawText("TEST ATLAS - F1 to open", 8, 8, TEST_FONT_SIZE, RAYWHITE);
    DrawStateName(STATE_PLAYING);
}

static void DrawGameOver(const Game* game)
{
    DrawText("TEST ATLAS - F1 to open", 8, 8, TEST_FONT_SIZE, RAYWHITE);
    DrawStateName(STATE_GAME_OVER);
}

static void DrawTest(const Game* game)
{
    for (int y = 0; y < SCREEN_HEIGHT; y += TEST_CHECKER_SIZE)
    {
        for (int x = 0; x < SCREEN_WIDTH; x += TEST_CHECKER_SIZE)
        {
            bool light = ((x + y) / TEST_CHECKER_SIZE) % 2 == 0;
            DrawRectangle(x, y, TEST_CHECKER_SIZE, TEST_CHECKER_SIZE, light ? RAYWHITE : LIGHTGRAY);
        }
    }

    DrawText("TEST ATLAS - F1 to close", 8, 8, TEST_FONT_SIZE, BLACK);

    for (int i = 0; i < SPRITE_COUNT; i++)
    {
        int col = i % TEST_COLUMNS;
        int row = i / TEST_COLUMNS;

        Vector2 center =
        {
            col * TEST_SLOT_WIDTH + TEST_SLOT_WIDTH * 0.5f,
            TEST_TOP + row * TEST_SLOT_HEIGHT + TEST_SLOT_HEIGHT * 0.5f,
        };
        AtlasDraw(&game->atlas, (SpriteId)i, center, WHITE);
    }
}

void GameDraw(const Game* game)
{
    ClearBackground(BLACK);

    switch (game->state)
    {
        case STATE_SPLASH:
            DrawSplash(game);
            break;

        case STATE_MENU:
            DrawMenu(game);
            break;

        case STATE_PLAYING:
            DrawPlaying(game);
            break;

        case STATE_GAME_OVER:
            DrawGameOver(game);
            break;

        case STATE_TEST:
            DrawTest(game);
            break;

        case STATE_COUNT: break;
    }
}

#pragma endregion