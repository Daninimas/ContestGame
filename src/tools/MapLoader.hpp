#pragma once

#include <eng/GameEngine.hpp>
#include <tools/Vector2.hpp>
#include <tools/tileson.hpp>
#include <tools/WorldPhase.hpp>
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



	static inline std::map<std::string, GameObjectType> goTypeMap{    // Este mapa sirve para coger el gameobject del mapa que se esta generando y pasarlo a enum para poder crear la entidad en el entitiy manager
        // Miscellaneous
        { "NONE", GameObjectType::NONE },
        { "ERROR", GameObjectType::ERROR },

        // Player
        {"PLAYER_GENERAL", GameObjectType::PLAYER_GENERAL},

        // Obstacles
        { "WALL", GameObjectType::WALL },

        // Weapons
        { "M4", GameObjectType::M4 },
        { "KNIFE", GameObjectType::KNIFE },
        { "GRENADE_LAUNCHER", GameObjectType::GRENADE_LAUNCHER },
        { "LASER_GUN", GameObjectType::LASER_GUN },

        // Enemy types
        { "CHASER", GameObjectType::CHASER },
        { "CHASERJUMPER", GameObjectType::CHASERJUMPER },
        { "DISTANCE_ENEMY", GameObjectType::DISTANCE_ENEMY },
        { "TRANSFORM_ENEMY", GameObjectType::TRANSFORM_ENEMY },
        { "BOMBER_ENEMY", GameObjectType::BOMBER_ENEMY },
        { "POUNCER_ENEMY", GameObjectType::POUNCER_ENEMY },

        // Power ups
        { "POWERUP_SHIELD", GameObjectType::POWERUP_SHIELD },
        { "POWERUP_FURY", GameObjectType::POWERUP_FURY },
	};

    static inline std::map<std::string, uint8_t> directionMap{    // Este mapa sirve para coger la direccion del mapa que se esta generando y pasarlo a enum 
        { "Left", WorldPhase::Left },
        { "Right", WorldPhase::Right },
        { "Up", WorldPhase::Up },
        { "Down", WorldPhase::Down },
    };
};

