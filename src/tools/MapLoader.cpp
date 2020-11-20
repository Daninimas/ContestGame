#include "MapLoader.hpp"

#include <iostream>


const bool MapLoader::loadMap(GameEngine& gameContext, const std::string mapPath) {

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        for (auto& phaseLayer : map->getLayers()) { // this are the layerGroups that are thr phases of the map
            std::cout << "Reading phase: " << phaseLayer.getName() << "\n";
            //Gets the layer from the .json map
            auto& objectLayers = phaseLayer.getLayers(); 

            for (tson::Layer& objLayer : objectLayers) { //This are the Objects Layer
                checkObjectsOfLayer(gameContext, objLayer);                
            }
        }
    }
    else {
        std::cout << "[Load Map Error] " << map->getStatusMessage() << "\n";
    }

	return true;
}



void MapLoader::checkObjectsOfLayer(GameEngine& gameContext, tson::Layer& objLayer) { // refactorizar esto, no hacer que por cata type haya que crear un metodo nuevo
    std::string layerName = objLayer.getName();
    std::cout << "  Reading Type layer: " << layerName << "\n";

    for (auto& obj : objLayer.getObjects())
    {
        if (obj.getObjectType() == tson::ObjectType::Object) {
            createObject(gameContext, layerName, obj);            
        }
    }
}


void MapLoader::createObject(GameEngine& gameContext, std::string layerName, tson::Object& obj) {
    std::string objType = obj.getType();

    std::cout << "      - Generate: " << objType << "\n";
    GameObjectType goType = getGameObject(objType);
    tson::Vector2f position = obj.getPosition();
    float rotation = obj.getRotation();


    if (goType != GameObjectType::ERROR) {

        // Check the object to crete
        if (layerName == "WALL") {
            gameContext.entityMan.createWall(gameContext, Vector2(position.x, position.y), rotation, goType);
        }
        else if (layerName == "ENEMY") {
            int enemyId = gameContext.entityMan.createEnemy(gameContext, Vector2(position.x, position.y), rotation, goType);

            setEnemyObjective(gameContext, enemyId);
        }
        else if (layerName == "PLAYER") {
            gameContext.entityMan.createPlayer(gameContext, Vector2(position.x, position.y), rotation, goType);
        }
        else if (layerName == "SPAWNER") {
            int spawnId = gameContext.entityMan.createSpawner(gameContext, Vector2(position.x, position.y), rotation, goType);

            gameContext.entityMan.getComponent<SpawnerComponent>(spawnId).objectiveId = WorldData::playerId;
        }
        else if (layerName == "POWERUP") {
            gameContext.entityMan.createPowerUp(gameContext, Vector2(position.x, position.y), rotation, goType);
        }
    }
    else {
        // Error on the type of the object
        //gameContext.entityMan.createError(gameContext, Vector2(position.x, position.y), rotation, goType);
    }
}


GameObjectType MapLoader::getGameObject(const std::string objType) { // if not found, return GameObject ERROR
    auto it = goTypeMap.find(objType);

    if (it != goTypeMap.end())
        return it->second;
    else
        return GameObjectType::ERROR;
}


void MapLoader::setEnemyObjective(GameEngine& gameContext, int enemyId) {
    if (gameContext.entityMan.existsComponent<AIChaseComponent>(enemyId))
        gameContext.entityMan.getComponent<AIChaseComponent>(enemyId).objectiveId = WorldData::playerId;

    if (gameContext.entityMan.existsComponent<AIMeleeAtkComponent>(enemyId))
        gameContext.entityMan.getComponent<AIMeleeAtkComponent>(enemyId).objectiveId = WorldData::playerId;

    if (gameContext.entityMan.existsComponent<AIDistanceAtkComponent>(enemyId))
        gameContext.entityMan.getComponent<AIDistanceAtkComponent>(enemyId).objectiveId = WorldData::playerId;

    if (gameContext.entityMan.existsComponent<AITransformationComponent>(enemyId))
        gameContext.entityMan.getComponent<AITransformationComponent>(enemyId).objectiveId = WorldData::playerId;

    if (gameContext.entityMan.existsComponent<AIPounceComponent>(enemyId))
        gameContext.entityMan.getComponent<AIPounceComponent>(enemyId).objectiveId = WorldData::playerId;
}