#pragma once

enum class GameObjectType {
    NONE,
    PLAYER_GENERAL,

    // Attacks
    PLAYER_MELEE_ATTACK,
    MELEE_ATTACK,
    DISTANCE_ATTACK,
    PLAYER_DISTANCE_ATTACK,
    EXPLOSION,
    PLAYER_LASER,
    LASER,


    // Weapons
    PISTOL,
    M4,
    KNIFE,
    GRENADE_LAUNCHER,
    LASER_GUN,


    // Menu types
    PAUSE,
    MAINMENU,
    OPTIONS_CONTROLS,


    // Enemy types
    CHASER,
    CHASERJUMPER,
    DISTANCE_ENEMY,
    TRANSFORM_ENEMY,
    BOMBER_ENEMY,

    // Bombs
    BOMB,
    PLAYER_BOMB
};