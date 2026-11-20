#include "core/game.h"
#include "config.h"
#include "raylib/raylib.h"

static const char* const STATE_NAMES[] = {
    [STATE_SPLASH]      = "SPLASH",
    [STATE_MENU]        = "MENU",
    [STATE_PLAYING]     = "PLAYING",
    [STATE_GAME_OVER]   = "GAME OVER",
};

static constexpr int STATE_NAME_FONT_SIZE = 40;

static void LoadWindowIcon(void)
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

    // TODO: Remember to allocate here

    return true;
}

void GameDeinit(Game* game)
{
    if (game == nullptr) return;

    // TODO: Remember to deallocate here

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

void GameUpdate(Game* game, float deltaTime)
{
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

        case STATE_COUNT: break;
    }
}

#pragma endregion

#pragma region Draw

static void DrawStateName(GameState state)
{
    const char *name = STATE_NAMES[state];
    int width = MeasureText(name, STATE_NAME_FONT_SIZE);
    DrawText(name,(SCREEN_WIDTH - width) / 2,(SCREEN_HEIGHT - STATE_NAME_FONT_SIZE) / 2, STATE_NAME_FONT_SIZE, RAYWHITE);
}

static void DrawSplash(const Game* game)
{
    DrawStateName(STATE_SPLASH);
}

static void DrawMenu(const Game* game)
{
    DrawStateName(STATE_MENU);
}

static void DrawPlaying(const Game* game)
{
    DrawStateName(STATE_PLAYING);
}

static void DrawGameOver(const Game* game)
{
    DrawStateName(STATE_GAME_OVER);
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

        case STATE_COUNT: break;
    }
}

#pragma endregion