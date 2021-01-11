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

    // Attacks
    PLAYER_MELEE_ATTACK,
    MELEE_ATTACK,
    DISTANCE_ATTACK,
    PLAYER_DISTANCE_ATTACK,
    EXPLOSION,
    PLAYER_LASER,
    LASER,

    // Obstacles
    WALL,


    // Weapons
    M4,
    KNIFE,
    GRENADE_LAUNCHER,
    LASER_GUN,


    // Menu types
    PAUSE,
    MAINMENU,
    CONTROLS_KEYBOARD,
    CONTROLS_JOYSTICK,
    GAME_OVER_MENU,


    // Enemy types
    CHASER,
    CHASERJUMPER,
    DISTANCE_ENEMY,
    TRANSFORM_ENEMY,
    BOMBER_ENEMY,
    POUNCER_ENEMY,

    // Bombs
    BOMB,
    PLAYER_BOMB,

    // Power ups
    SHIELD,
    PLAYER_SHIELD,
    POWERUP_SHIELD,
    POWERUP_FURY,

    //HUD elements
    HUD_PLAYER_HEALTH,
    HUD_PLAYER_AMMO,
    HUD_PLAYER_LIFES,

    // World
    WORLD_DEBUG,

    // Damage platforms
    DAMAGE_PLATFORM,

    // Triggers
    TRIGGER
    /*TRIGGER_SOUND,
    TRIGGER_CREATE_ENTITY,
    TRIGGER_FUNCTION*/

};