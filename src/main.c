#include "config.h"
#include "core/game.h"
#include "raylib/raylib.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "1945");
    SetTargetFPS(TARGET_FPS);
    InitAudioDevice();

    Game game;
    if (!GameInit(&game))
    {
        CloseAudioDevice();
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose() && !GameShouldQuit(&game))
    {
        GameUpdate(&game, GetFrameTime());

        BeginDrawing();
        GameDraw(&game);
        EndDrawing();
    }

    GameDeinit(&game);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
