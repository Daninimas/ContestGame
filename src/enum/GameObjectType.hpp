#pragma once

///////////////////////////////////////////////////////////////////////
//                                                                   //
//                            IMPORTANTE                             //
//                                                                   //
//   Si se crea un tipo nuevo, anyadirlo en el mapa del MapLoader    //
//                                                                   //
///////////////////////////////////////////////////////////////////////

enum class GameObjectType {
    // Miscellaneous
    NONE,
    ERROR,

    // Player
    PLAYER_GENERAL,
    PLAYER_HAND,

    // Attacks
    PLAYER_MELEE_ATTACK,
    MELEE_ATTACK,
    DISTANCE_ATTACK,
    PLAYER_DISTANCE_ATTACK,
    EXPLOSION,
    PLAYER_EXPLOSION,
    PLAYER_LASER,
    LASER,

    // Obstacles
    PLATFORM,
    WALL,
    FALLING_WALL,


    // Weapons
    M4,
    KNIFE,
    GRENADE_LAUNCHER,
    LASER_GUN,
    SHOTGUN,
    PISTOL,


    // Menu types
    PAUSE,
    MAINMENU,
    CONTROLS_KEYBOARD,
    CONTROLS_JOYSTICK,
    GAME_OVER_MENU,
    BEST_SCORES,
    NEW_BEST_SCORE,


    // Enemy types
    CHASER,
    CHASERJUMPER,
    DISTANCE_ENEMY,
    DISTANCE_WALKING_ENEMY,
    TRANSFORM_ENEMY,
    BOMBER_ENEMY,
    POUNCER_ENEMY,
    ENEMY_SPIDER,
    FLYING_SHOOTER_ENEMY,
    FIRST_ENEMY,  //El enemigo que te da la pistola

    // Drone types
    DRONE_FRIEND,
    DRONE_ENEMY,

    // Bombs
    BOMB,
    PLAYER_BOMB,

    // Power ups
    SHIELD,
    PLAYER_SHIELD,
    POWERUP_SHIELD,
    POWERUP_FURY,
    POWERUP_EXTRA_LIFE,
    POWERUP_DRONE,

    //HUD elements
    HUD_PLAYER_HEALTH,
    HUD_PLAYER_AMMO,
    HUD_PLAYER_LIFES,
    HUD_PLAYER_SCORE,

    // World
    WORLD_DEBUG,
    WORLD_1,
    WORLD_2,
    WORLD_3,

    // Damage platforms
    DAMAGE_PLATFORM,

    // Triggers
    TRIGGER,
    /*TRIGGER_SOUND,
    TRIGGER_CREATE_ENTITY,
    TRIGGER_FUNCTION*/


    // Orbital marker
    ORBITAL_MARKER,

    // Orbital striker
    ORBITAL_STRIKER,

    // Turret
    TURRET_GUN,
    TURRET_PLATFORM,

    // Spawner
    SPAWNER,

    // Child
    CHILD,

    // Text
    TEXT
};