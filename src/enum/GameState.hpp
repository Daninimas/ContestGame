#pragma once

enum class GameState {
    NOSTATE,
    PLAYING,
    GAMEOVER,
    PAUSE,
    CONTROLS,
    WAIT_INPUT,
    WAIT_AFTER_LOSE_LIFE
};