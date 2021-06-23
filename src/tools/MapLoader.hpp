#pragma once

#include <eng/GameEngine.hpp>
#include <tools/Vector2.hpp>
#include <tools/tileson.hpp>
#include <tools/WorldPhase.hpp>
#include <enum/TriggerFunction.hpp>
#include <string>
#include <map>


class MapLoader {
public:
    static const bool loadMap(GameEngine& gameContext, const std::string mapPath);
    static const bool loadMapPhase(GameEngine& gameContext, const std::string mapPath, const std::string phaseName);
    static uint8_t getNumberOfPhases(const std::string mapPath);

private:
	static void checkObjectsOfLayer(GameEngine& gameContext, tson::Layer& objLayer, tson::Layer* phaseLayer);
    static void createObject(GameEngine& gameContext, std::string layerName, tson::Object& obj);
    static void setPhaseData(GameEngine& gameContext, tson::Object& obj, tson::Layer* phaseLayer);
    static GameObjectType getGameObject(const std::string objType);
    static uint8_t getDirection(const std::string dir);
    static void setEnemyObjective(GameEngine& gameContext, int enemyId);
    static void setTriggerData(GameEngine& gameContext, int triggerId, tson::Object& obj);
    static void setSpawnerData(GameEngine& gameContext, int spawnerId, tson::Object& obj);
    static TriggerFunction getTriggerFunction(const std::string func);
    static uint8_t getFacing(const std::string facingString);



	static inline std::map<std::string, GameObjectType> goTypeMap{    // Este mapa sirve para coger el gameobject del mapa que se esta generando y pasarlo a enum para poder crear la entidad en el entitiy manager
        // Miscellaneous
        { "NONE", GameObjectType::NONE },
        { "ERROR", GameObjectType::ERROR },

        // Player
        {"PLAYER_GENERAL", GameObjectType::PLAYER_GENERAL},

        // Obstacles
        { "WALL", GameObjectType::WALL },
        { "PLATFORM", GameObjectType::PLATFORM },
        { "FALLING_WALL", GameObjectType::FALLING_WALL },

        // Weapons
        { "M4", GameObjectType::M4 },
        { "KNIFE", GameObjectType::KNIFE },
        { "GRENADE_LAUNCHER", GameObjectType::GRENADE_LAUNCHER },
        { "LASER_GUN", GameObjectType::LASER_GUN },
        { "SHOTGUN", GameObjectType::SHOTGUN },
        { "PISTOL", GameObjectType::PISTOL },

        // Enemy types
        { "CHASER", GameObjectType::CHASER },
        { "CHASERJUMPER", GameObjectType::CHASERJUMPER },
        { "DISTANCE_ENEMY", GameObjectType::DISTANCE_ENEMY },
        { "DISTANCE_WALKING_ENEMY", GameObjectType::DISTANCE_WALKING_ENEMY },
        { "TRANSFORM_ENEMY", GameObjectType::TRANSFORM_ENEMY },
        { "BOMBER_ENEMY", GameObjectType::BOMBER_ENEMY },
        { "POUNCER_ENEMY", GameObjectType::POUNCER_ENEMY },
        { "ENEMY_SPIDER", GameObjectType::ENEMY_SPIDER },
        { "FLYING_SHOOTER_ENEMY", GameObjectType::FLYING_SHOOTER_ENEMY },
        { "FIRST_ENEMY", GameObjectType::FIRST_ENEMY },
        

        // Power ups
        { "POWERUP_SHIELD", GameObjectType::POWERUP_SHIELD },
        { "POWERUP_FURY", GameObjectType::POWERUP_FURY },
        { "POWERUP_EXTRA_LIFE", GameObjectType::POWERUP_EXTRA_LIFE },

        // Triggers
        { "TRIGGER", GameObjectType::TRIGGER },

        // Damage platforms
        { "DAMAGE_PLATFORM", GameObjectType::DAMAGE_PLATFORM },

        // Drone
        { "DRONE_FRIEND", GameObjectType::DRONE_FRIEND},

        // Child
        { "CHILD", GameObjectType::CHILD}
    };

    static inline std::map<std::string, uint8_t> directionMap{    // Este mapa sirve para coger la direccion del mapa que se esta generando y pasarlo a enum 
        { "Left", WorldPhase::Left },
        { "Right", WorldPhase::Right },
        { "Up", WorldPhase::Up },
        { "Down", WorldPhase::Down }
    };

    static inline std::map<std::string, TriggerFunction> triggerFuncMap{    // Este mapa sirve para coger la funcion de trigger que se esta generando y pasarlo a enum 
        { "PLAY_MUSIC", TriggerFunction::PLAY_MUSIC },
        { "PLAY_SOUND", TriggerFunction::PLAY_SOUND },
        { "CREATE_ENTITY", TriggerFunction::CREATE_ENTITY },

        { "SPAWN_CITY_1", TriggerFunction::SPAWN_CITY_1 }
    };

    static inline std::map<std::string, uint8_t> facingMap {    // Este mapa sirve para saber desde un string el facing 
        { "Left", SituationComponent::Left },
        { "Right", SituationComponent::Right }
    };
};

