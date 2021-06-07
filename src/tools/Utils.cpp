#include "Utils.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

#include <enum/Controls.hpp>

#include <tools/json.hpp>
using json = nlohmann::json;
#include <fstream> 

Utils::Utils() {}

float Utils::stringToFloat(std::string string) {
    float n {0.f};
    std::string delimiter = ".";
    size_t delIndex = string.find(delimiter);
    std::string first = string.substr(0, delIndex);

    size_t length = first.size() - 1;

    for (char ch : first) {
        int exp = 1;
        for (size_t i = 0; i < length; ++i)
            exp *= 10;
        n += exp * (ch - 48);
        --length;
    }

    size_t current = 0;
    if (delIndex < string.size()) {
        std::string second = string.substr(delIndex + 1, string.size());
        length = second.size() - 1;

        for (char ch : second) {
            float exp = 1;
            for (size_t i = 0; i <= current; ++i)
                exp /= 10;
            n += exp * (ch - 48);
            current++;
        }
    }
    return n;
}

int Utils::stringToInt(std::string string) {
    int n {0};
    size_t length = string.size() - 1;

    for (char ch : string) {
        int exp = 1;
        for (size_t i = 0; i < length; ++i)
            exp *= 10;
        n += exp * (ch - 48);
        --length;
    }
    return n;
}

float Utils::calculateDistance(const float posX1, const float posX2, const float posY1, const float posY2) {
    float distance = sqrt((posX1 - posX2) * (posX1 - posX2) + (posY1 - posY2) * (posY1 - posY2));
    return distance;
}

float Utils::degToRad(float angle) {
    float ret = 0;
    ret = angle / 180.f * 3.14159f;
    return ret;
}

float Utils::radToDeg(float angle) {
    float ret = 0;
    ret = angle * 180.f / 3.14159f;
    return ret;
}


BoundingBox Utils::moveToWorldCoords(BoundingBox& bounding, SituationComponent& situation) {
    BoundingBox worldCoords{
            bounding.xLeft  + situation.position.x
        ,   bounding.xRight + situation.position.x
        ,   bounding.yUp    + situation.position.y
        ,   bounding.yDown  + situation.position.y
    };

    return worldCoords;
}


void Utils::insertCollidersIdWithVelocity(GameEngine& gameContext, std::vector<int>& idCollidersWithVelocity) {
    auto& velocitiesComps = gameContext.entityMan.getComponents<VelocityComponent>();
    auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();
    for (auto& velComp : velocitiesComps) {
        int idVelEnt = velComp.id;

        if (colliders.exists(idVelEnt)) {
            // has the two components
            idCollidersWithVelocity.emplace_back(idVelEnt);
        }
    }
}

void Utils::insertNotWallColliders(GameEngine& gameContext, std::vector<std::reference_wrapper<ColliderComponent>>& collidersNotWall) {
    auto& colliders = gameContext.entityMan.getComponents<ColliderComponent>();
    for (auto& collComp : colliders) {
        if (collComp.collisionLayer != ColliderComponent::Wall && collComp.collisionLayer != ColliderComponent::Platform && collComp.collisionLayer != ColliderComponent::Trigger) {
            collidersNotWall.emplace_back(collComp);
        }
    }
}


bool Utils::objectiveInsideRange(SituationComponent& attackerSit, SituationComponent& objectiveSit, const Vector2 range) {
    if (abs(attackerSit.position.x - objectiveSit.position.x) < range.x && abs(attackerSit.position.y - objectiveSit.position.y) < range.y) {
        return true;
    }
    return false;
};


bool Utils::isEntitySensoredBy(SensorComponent& sensor, const int sensoredEntity) {

    for (int sensedId : sensor.entitiesSensoring) {
        if (sensedId == sensoredEntity) {
            return true;
        }
    }

    return false;
}


bool Utils::checkCollidingWithEntity(BoundingBoxNode& boundingNode, int objId) {
    bool collide = false;
    std::vector<int>& entitiesColliding = boundingNode.bounding.entitiesColliding;

    for (auto& b : boundingNode.childs) {
        collide = checkCollidingWithEntity(b, objId);
    }

    if (std::find(entitiesColliding.begin(), entitiesColliding.end(), objId) != entitiesColliding.end()) {
        collide = true;
    }

    return collide;
}


void Utils::deleteCollidingWithObjective(BoundingBoxNode& boundingNode, int objId) {
    std::vector<int>& entitiesColliding = boundingNode.bounding.entitiesColliding;

    if (boundingNode.childs.size() > 0) {
        for (auto& b : boundingNode.childs) {
            deleteCollidingWithObjective(b, objId);
        }
    }

    entitiesColliding.erase(std::remove(entitiesColliding.begin(), entitiesColliding.end(), objId), entitiesColliding.end());
}



void Utils::setNormalPistolToEntity(GameEngine& gameContext, int entityId) {
    DistanceWeaponComponent& distanceWeaponComp = gameContext.entityMan.createComponent<DistanceWeaponComponent>(entityId);

    distanceWeaponComp.attackBounding = { 0.f, 2.f, 0.f, 2.f };
    distanceWeaponComp.damage = 1;
    distanceWeaponComp.attackGeneralVelociy = 500.f;
    distanceWeaponComp.attackGravity = 100.f;
    distanceWeaponComp.maxCooldown = 0.5f;
    distanceWeaponComp.attackGeneratedType = DistanceWeaponComponent::BULLET;
    distanceWeaponComp.attackSound.soundPath = "Media/Sound/Weapons/gunFire.wav";
    distanceWeaponComp.ammo = 0;
    distanceWeaponComp.infiniteAmmo = true;
    distanceWeaponComp.bulletSpreadAngle = 5.f;
    distanceWeaponComp.spawnAttackPos = { 20.f, 39.f };
}


Vector2 Utils::getCenterOfBounding(BoundingBox const bounding) {
    Vector2 center;
    center.x = (bounding.xLeft + bounding.xRight) / 2.f;
    center.y = (bounding.yUp + bounding.yDown) / 2.f;

    return center;
}


void Utils::printBoundingBox(BoundingBox const bounding) {
    std::cout << "xLeft: " << bounding.xLeft << " xRight: " << bounding.xRight << " yUp: " << bounding.yUp << " yDown: " << bounding.yDown << "\n";
}


BoundingBox Utils::getCameraViewBoundig(CameraComponent& cameraComp) {
    float zoomedRectX = cameraComp.viewRect.x * cameraComp.zoom;
    float zoomedRectY = cameraComp.viewRect.y * cameraComp.zoom;

    return BoundingBox{ -(zoomedRectX / 2.f), zoomedRectX / 2.f, -(zoomedRectY / 2.f), zoomedRectY / 2.f };
}


bool Utils::entityInPhaseLimit(GameEngine& gameContext, SituationComponent& entitySit) {
    WorldPhase& phase = gameContext.entityMan.getComponent<WorldComponent>(WorldElementsData::worldId).currentPhase;

    switch (phase.direction)
    {
    case WorldPhase::Left:
        if (entitySit.position.x < (phase.limits.xLeft + phase.endDistance)) {
            return true;
        }
        break;

    case WorldPhase::Right:
        if (entitySit.position.x > (phase.limits.xRight - phase.endDistance)) {
            return true;
        }
        break;

    case WorldPhase::Up:
        if (entitySit.position.y < (phase.limits.yUp + phase.endDistance)) {
            return true;
        }
        break;

    case WorldPhase::Down:
        if (entitySit.position.y > (phase.limits.yDown - phase.endDistance)) {
            return true;
        }
        break;
    }

    return false;
}


void Utils::setPhaseStartToView(GameEngine& gameContext, uint8_t lastPhaseDirection) {
    WorldPhase&      phase  = gameContext.entityMan.getComponent<WorldComponent>(WorldElementsData::worldId).currentPhase;
    CameraComponent& camera = gameContext.entityMan.getComponent<CameraComponent>(WorldElementsData::activeCameraId);
    BoundingBox cameraView  = getCameraViewBoundig(camera);

    cameraView = moveToWorldCoords(cameraView, gameContext.entityMan.getComponent<SituationComponent>(WorldElementsData::activeCameraId));

    switch (lastPhaseDirection)
    {
    case WorldPhase::Left:
        phase.limits.xRight = cameraView.xRight;
        break;

    case WorldPhase::Right:
        phase.limits.xLeft = cameraView.xLeft;
        break;

    case WorldPhase::Up:
        phase.limits.yDown = cameraView.yDown;
        break;

    case WorldPhase::Down:
        phase.limits.yUp = cameraView.yUp;
        break;
    }
}


void Utils::setControlKeyToMenuOptions(GameEngine& gameContext, MenuComponent& controlsMenu) {

    if (gameContext.entityMan.getEntity(controlsMenu.id).getGameObjectType() == GameObjectType::CONTROLS_KEYBOARD || gameContext.entityMan.getEntity(controlsMenu.id).getGameObjectType() == GameObjectType::CONTROLS_JOYSTICK) {
        for (int id : controlsMenu.optionsId) {
            MenuOptionComponent& optionComp = gameContext.entityMan.getComponent<MenuOptionComponent>(id);
            TextComponent& textComp = gameContext.entityMan.getComponent<TextComponent>(id);
            InputComponent& inputComp = gameContext.entityMan.getComponent<InputComponent>(WorldElementsData::playerId);

            switch (optionComp.option)
            {
            case MenuOptions::SET_KEY_ATTACK:
                textComp.text = "ATTACK: " + keyNameMap.at(inputComp.keyboardControlsMap[Controls::ACTION]);

                break;

            case MenuOptions::SET_KEY_JUMP:
                textComp.text = "JUMP: " + keyNameMap.at(inputComp.keyboardControlsMap[Controls::JUMP]);
                break;

            case MenuOptions::SET_KEY_LEFT:
                textComp.text = "LEFT: " + keyNameMap.at(inputComp.keyboardControlsMap[Controls::MOVE_LEFT]);
                break;

            case MenuOptions::SET_KEY_RIGHT:
                textComp.text = "RIGHT: " + keyNameMap.at(inputComp.keyboardControlsMap[Controls::MOVE_RIGHT]);
                break;

            case MenuOptions::SET_KEY_UP:
                textComp.text = "UP: " + keyNameMap.at(inputComp.keyboardControlsMap[Controls::MOVE_UP]);
                break;

            case MenuOptions::SET_KEY_DOWN:
                textComp.text = "DOWN: " + keyNameMap.at(inputComp.keyboardControlsMap[Controls::MOVE_DOWN]);
                break;


            // For the joystick
            case MenuOptions::SET_JOYSTICK_ATTACK:
                textComp.text = "ATTACK: Button " + to_string(inputComp.keyboardControlsMap[Controls::JOYSTICK_ACTION]);
                break;

            case MenuOptions::SET_JOYSTICK_JUMP:
                textComp.text = "JUMP: Button " + to_string(inputComp.keyboardControlsMap[Controls::JOYSTICK_JUMP]);
                break;
            } 
        }

        gameContext.getWindowFacadeRef().updateEntities(gameContext, controlsMenu.optionsId);
    }
}


std::string Utils::getKeyName(uint8_t keyCode) {
    return keyNameMap.at(keyCode);
}


void Utils::resetPlayerPosition(GameEngine& gameContext) {
    SituationComponent& playerSituation = gameContext.entityMan.getComponent<SituationComponent>(WorldElementsData::playerId);

    SituationComponent* closesWall = getClosestWallXToObjetive(gameContext, playerSituation);
    if (closesWall != nullptr) {
        ColliderComponent& playerColl = gameContext.entityMan.getComponent<ColliderComponent>(WorldElementsData::playerId);

        playerSituation.position.x = closesWall->position.x;
        playerSituation.position.y = closesWall->position.y - (playerColl.boundingRoot.bounding.yDown - playerColl.boundingRoot.bounding.yUp) - 100.f;
    }
    else {
        gameContext.pushGameState(GameState::GAMEOVER);
    }
}

SituationComponent* Utils::getClosestWallXToObjetive(GameEngine& gameContext, SituationComponent& objetiveSituation, bool onlyWALLGameObject) {
    auto& allSituations = gameContext.entityMan.getComponents<SituationComponent>();
    SituationComponent* closesWall = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    // This should be the right corner of the objective bounding
    float objectivePositionX = objetiveSituation.position.x;
    if (gameContext.entityMan.existsComponent<ColliderComponent>(objetiveSituation.id)) {
        objectivePositionX += gameContext.entityMan.getComponent<ColliderComponent>(objetiveSituation.id).boundingRoot.bounding.xRight;
    }
    
    for (SituationComponent& entitySit : allSituations) {
        if (onlyWALLGameObject && gameContext.getEntity(entitySit.id).getGameObjectType() != GameObjectType::WALL) {
            continue;
        }
        if (gameContext.getEntity(entitySit.id).getType() == EntityType::WALL) {
            float distance = objectivePositionX - entitySit.position.x;
            if (entitySit.position.x < objectivePositionX && distance < closestDistance) {
                closestDistance = distance;
                closesWall = &entitySit;
            }
        }
    }

    return closesWall;
}

float Utils::getVectorMagnitude(Vector2& vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y); // Calculate magnitude
}

Vector2 Utils::rotateVector(Vector2& vect, float angle) {
    Vector2 rotatedAngle;

    rotatedAngle.x = vect.x * cos(degToRad(angle)) - vect.y * sin(degToRad(angle));
    rotatedAngle.y = vect.x * sin(degToRad(angle)) + vect.y * cos(degToRad(angle));

    return rotatedAngle;
}


void Utils::updateControlsJSON(InputComponent& inputComp) {
    //std::ifstream i("config.json");
    json jf;
    jf["MOVE_LEFT"] = inputComp.keyboardControlsMap[Controls::MOVE_LEFT];
    jf["MOVE_RIGHT"] = inputComp.keyboardControlsMap[Controls::MOVE_RIGHT];
    jf["MOVE_UP"] = inputComp.keyboardControlsMap[Controls::MOVE_UP];
    jf["MOVE_DOWN"] = inputComp.keyboardControlsMap[Controls::MOVE_DOWN];
    jf["ACTION"] = inputComp.keyboardControlsMap[Controls::ACTION];
    jf["JUMP"] = inputComp.keyboardControlsMap[Controls::JUMP];
    jf["JOYSTICK_ACTION"] = inputComp.keyboardControlsMap[Controls::JOYSTICK_ACTION];
    jf["JOYSTICK_JUMP"] = inputComp.keyboardControlsMap[Controls::JOYSTICK_JUMP];

    std::ofstream file("config.json");
    file << jf;
}