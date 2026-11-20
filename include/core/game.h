#pragma once

typedef enum GameState
{
    STATE_SPLASH,
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_COUNT
} GameState;

typedef struct Game
{
    GameState state;
    int menuIndex;
    int score;
    int highScore;
    bool quit;
} Game;

bool GameInit(Game* game);
void GameDeinit(Game* game);
void GameUpdate(Game* game, float deltaTime);
void GameDraw(const Game* game);
void GameChangeState(Game* game, GameState next);
bool GameShouldQuit(const Game* game);