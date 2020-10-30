#include "Utils.hpp"
#include <iostream>

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
            bounding.xLeft  + situation.x
        ,   bounding.xRight + situation.x
        ,   bounding.yUp    + situation.y
        ,   bounding.yDown  + situation.y
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


bool Utils::objectiveInsideRange(SituationComponent& attackerSit, SituationComponent& objectiveSit, float rangeX, float rangeY) {
    if (abs(attackerSit.x - objectiveSit.x) < rangeX && abs(attackerSit.y - objectiveSit.y) < rangeY) {
        return true;
    }
    return false;
};