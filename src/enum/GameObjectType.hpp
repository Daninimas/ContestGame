#pragma once

enum class GameObjectType {
    NONE,
    PLAYER_GENERAL,
    PLAYER_MELEE_ATTACK,
    MELEE_ATTACK,
    DISTANCE_ATTACK,
    PLAYER_DISTANCE_ATTACK,


    // Weapons
    PISTOL,
    M4,
    KNIFE,


    // Menu types
    PAUSE,
    MAINMENU,
    OPTIONS_CONTROLS,


    // Enemy types
    CHASER
};