#include "MapLoader.hpp"

#include <iostream>


const bool MapLoader::loadMap(GameEngine& gameContext, const std::string mapPath) {

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        for (auto& phaseLayer : map->getLayers()) { // this are the layerGroups that are the phases of the map
            std::cout << "Reading phase: " << phaseLayer.getName() << "\n";
            //Gets the layer from the .json map
            auto& objectLayers = phaseLayer.getLayers(); 

            for (tson::Layer& objLayer : objectLayers) { //This are the Objects Layer
                checkObjectsOfLayer(gameContext, objLayer, &phaseLayer);
            }
        }
    }
    else {
        std::cout << "[Load Map Error] " << map->getStatusMessage() << "\n";
    }

	return true;
}


const bool MapLoader::loadMapPhase(GameEngine& gameContext, const std::string mapPath, const std::string phaseName) {

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        tson::Layer* phaseLayer = map->getLayer(phaseName);
        if (phaseLayer != nullptr) { // this are the layerGroups that are the phases of the map
            std::cout << "Reading phase: " << phaseLayer->getName() << "\n";
            //Gets the layer from the .json map
            auto& objectLayers = phaseLayer->getLayers();

            for (tson::Layer& objLayer : objectLayers) { //This are the Objects Layer
                checkObjectsOfLayer(gameContext, objLayer, phaseLayer);
            }
        }
        else {
            std::cout << "[Load Layer Error] The layer ( " << phaseName << " ) doesn't exist in the map (" << mapPath << ")\n";
        }
    }
    else {
        std::cout << "[Load Map Error] " << map->getStatusMessage() << "\n";
    }

    return true;
}



void MapLoader::checkObjectsOfLayer(GameEngine& gameContext, tson::Layer& objLayer, tson::Layer* phaseLayer) {
    std::string layerName = objLayer.getName();
    std::cout << "  Reading Type layer: " << layerName << "\n";

    for (auto& obj : objLayer.getObjects())
    {
        if (layerName != "phase_size") {
            createObject(gameContext, layerName, obj);
        }
        else {
            setPhaseData(gameContext, obj, phaseLayer);
        }
    }
}


void MapLoader::setPhaseData(GameEngine& gameContext, tson::Object& obj, tson::Layer* phaseLayer) {
    std::string objType = obj.getType();

    tson::Vector2f position = obj.getPosition();
    tson::Vector2f size = obj.getSize();
    float rotation = obj.getRotation();

    WorldPhase newPhase = WorldPhase();
    newPhase.limits = BoundingBox{ position.x, size.x + position.x, position.y, size.y + position.y };
    std::cout << "      - Phase limits: { " << newPhase.limits.xLeft << ", " << newPhase.limits.xRight << ", " << newPhase.limits.yUp << ", " << newPhase.limits.yDown << " }\n";

    newPhase.direction = getDirection( phaseLayer->get<std::string>("Direction") );
    newPhase.endDistance = 100.f;
    //newPhase.phaseMusic.path = ;
    //newPhase.phaseMusic.repeat = true;
    gameContext.entityMan.getComponent<WorldComponent>(WorldElementsData::worldId).currentPhase = std::move(newPhase);
}

void MapLoader::createObject(GameEngine& gameContext, std::string layerName, tson::Object& obj) {
    std::string objType = obj.getType();

    std::cout << "      - Generate: " << objType << "\n";
    GameObjectType goType = getGameObject(objType);
    tson::Vector2f position = obj.getPosition();
    tson::Vector2f size = obj.getSize();
    float rotation = obj.getRotation();


    if (goType != GameObjectType::ERROR) {

        // Check the object to create
        if (layerName == "WALL") {
            gameContext.entityMan.createWall(gameContext, Vector2(position.x, position.y), Vector2(size.x, size.y), rotation, goType);
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

            gameContext.entityMan.getComponent<SpawnerComponent>(spawnId).objectiveId = WorldElementsData::playerId;
        }
        else if (layerName == "POWERUP") {
            gameContext.entityMan.createPowerUp(gameContext, Vector2(position.x, position.y), rotation, goType);
        }
        else if (layerName == "TRIGGER") {
            int triggerId = gameContext.entityMan.createTrigger(gameContext, Vector2(position.x, position.y), rotation, goType);

            setTriggerData(gameContext, triggerId, obj);
        }
        else if (layerName == "WEAPON") {
            gameContext.entityMan.createWeapon(gameContext, Vector2(position.x, position.y), rotation, goType);
        }
        else if (layerName == "DAMAGE_PLATFORM") {
            uint16_t damage = obj.get<int>("Damage");
            gameContext.entityMan.createDamagePlatform(gameContext, Vector2(position.x, position.y), Vector2(size.x, size.y), damage, goType);
        }
        else if (layerName == "TURRET") {
            uint8_t facing = getFacing(obj.get<std::string>("Facing"));
            gameContext.entityMan.createTurret(gameContext, Vector2(position.x, position.y), facing);
        }
        else if (layerName == "DRONE") {
            gameContext.entityMan.createDrone(gameContext, Vector2(position.x, position.y), rotation, goType);
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

uint8_t MapLoader::getDirection(const std::string dir) { // if not found, return 0
    auto it = directionMap.find(dir);

    if (it != directionMap.end())
        return it->second;
    else {
        cout << "ERROR LOADING DIRECTION: " << dir << "\n";
        return 0;
    }
}

uint8_t MapLoader::getFacing(const std::string facingString) { // if not found, return 0
    auto it = facingMap.find(facingString);

    if (it != facingMap.end())
        return it->second;
    else {
        cout << "ERROR LOADING FACING: " << facingString << "\n";
        return 0;
    }
}

TriggerFunction MapLoader::getTriggerFunction(const std::string func) {
    auto it = triggerFuncMap.find(func);

    if (it != triggerFuncMap.end())
        return it->second;
    else {
        cout << "ERROR LOADING TRIGGER FUNCTION: " << func << "\n";
        return TriggerFunction::ERROR;
    }
}


void MapLoader::setEnemyObjective(GameEngine& gameContext, int enemyId) {
    if (gameContext.entityMan.existsComponent<AIChaseComponent>(enemyId))
        gameContext.entityMan.getComponent<AIChaseComponent>(enemyId).objectiveId = WorldElementsData::playerId;

    if (gameContext.entityMan.existsComponent<AIFlyingChaseComponent>(enemyId))
        gameContext.entityMan.getComponent<AIFlyingChaseComponent>(enemyId).objectiveId = WorldElementsData::playerId;

    if (gameContext.entityMan.existsComponent<AIMeleeAtkComponent>(enemyId))
        gameContext.entityMan.getComponent<AIMeleeAtkComponent>(enemyId).objectiveId = WorldElementsData::playerId;

    if (gameContext.entityMan.existsComponent<AIDistanceAtkComponent>(enemyId))
        gameContext.entityMan.getComponent<AIDistanceAtkComponent>(enemyId).objectiveId = WorldElementsData::playerId;

    if (gameContext.entityMan.existsComponent<AITransformationComponent>(enemyId))
        gameContext.entityMan.getComponent<AITransformationComponent>(enemyId).objectiveId = WorldElementsData::playerId;

    if (gameContext.entityMan.existsComponent<AIPounceComponent>(enemyId))
        gameContext.entityMan.getComponent<AIPounceComponent>(enemyId).objectiveId = WorldElementsData::playerId;
}


void MapLoader::setTriggerData(GameEngine& gameContext, int triggerId, tson::Object& obj) {
    ColliderComponent& collComp   = gameContext.entityMan.getComponent<ColliderComponent>(triggerId);
    TriggerComponent& triggerComp = gameContext.entityMan.getComponent<TriggerComponent>(triggerId);
    TriggerFunction triggerFunc = getTriggerFunction(obj.get<std::string>("Function"));


    if (triggerFunc != TriggerFunction::ERROR) {
        auto size = obj.getSize();
        collComp.boundingRoot.bounding = { 0.f, size.x, 0.f, size.y };

        triggerComp.functions.emplace_back(triggerFunc);

        switch (triggerFunc)
        {
        case TriggerFunction::PLAY_MUSIC:
            triggerComp.sound.soundPath = obj.get<std::string>("soundPath");
            triggerComp.sound.repeat = true;
            break;

        case TriggerFunction::PLAY_SOUND:
            triggerComp.sound.soundPath = obj.get<std::string>("soundPath");
            break;

        case TriggerFunction::CREATE_ENTITY:

            break;
        }
    
    }
    else {
        gameContext.eraseEntityByID(triggerId);
    }
}



uint8_t MapLoader::getNumberOfPhases(const std::string mapPath) {
    uint8_t numberOfPhases = 0;
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        for (auto& phaseLayer : map->getLayers()) { // this are the layerGroups that are the phases of the map
            if (phaseLayer.getType() == tson::LayerType::Group) {
                ++numberOfPhases;
            }
        }
    }
    else {
        std::cout << "[Count Phases of Map Error] " << map->getStatusMessage() << "\n";
    }

    return numberOfPhases;
}