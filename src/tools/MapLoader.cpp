#include "MapLoader.hpp"

#include <iostream>
#include <tools/Vector2.hpp>



const bool MapLoader::loadMap(GameEngine& gameContext, const std::string mapPath) {

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(mapPath));

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        for (auto& phaseLayer : map->getLayers()) { // this are the layerGroups that are thr phases of the map
            std::cout << phaseLayer.getName() << "\n";
            //Gets the layer from the .json map
            auto& objectLayers = phaseLayer.getLayers(); 

            for (tson::Layer& objLayer : objectLayers) { //This are the Objects Layer
                auto& layerName = objLayer.getName();
                std::cout << layerName << "\n";

                if (layerName == "WALL") {
                    createWalls(gameContext, objLayer);
                }
                else if (layerName == "ENEMY") {

                }
            }
        }
    }
    else {
        std::cout << "[Load Map Error] " << map->getStatusMessage() << "\n";
    }

	return true;
}



void MapLoader::createWalls(GameEngine& gameContext, tson::Layer& objLayer) { // refactorizar esto, no hacer que por cata type haya que crear un metodo nuevo

    for (auto& obj : objLayer.getObjects())
    {
        if (obj.getObjectType() == tson::ObjectType::Object) {
            tson::Vector2i position = obj.getPosition();
            float rotation = obj.getRotation();
            std::string objType = obj.getType();
            std::cout << objType << "\n";

            GameObjectType goType = getGameObject(objType);

            if (goType != GameObjectType::ERROR) {
                gameContext.entityMan.createWall(gameContext, Vector2(position.x, position.y), rotation, goType);
            }
            else {
                // Error on the type of the object
                //gameContext.entityMan.createError(gameContext, Vector2(position.x, position.y), rotation, goType);
            }
        }
    }
}


GameObjectType MapLoader::getGameObject(const std::string objType) { // if not found, return GameObject ERROR
    auto it = goTypeMap.find(objType);

    if (it != goTypeMap.end())
        return it->second;
    else
        return GameObjectType::ERROR;
}