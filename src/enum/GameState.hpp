#pragma once

enum class GameState {
    NOSTATE,
    PLAYING,
    GAMEOVER,
    PAUSE,
    CONTROLS,
    WAIT_INPUT,
    WAIT_AFTER_LOSE_LIFE,
    BEST_SCORES,
    NEW_BEST_SCORE,
    MAIN_MENU,
    NEXT_LEVEL,
    NEXT_LEVEL_MENU,
    WIN
};