#pragma once

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <string>


class Utils {
public:
    Utils();
    static float stringToFloat(std::string string);
    static int stringToInt(std::string string);

    static float    calculateDistance(const float posX1, const float posX2, const float posY1, const float posY2);
    static float    degToRad(float angle);
    static float    radToDeg(float angle);

    static BoundingBox moveToWorldCoords(BoundingBox& bounding, SituationComponent& situation);

    static void insertCollidersIdWithVelocity(GameEngine& gameContext, std::vector<int>& idCollidersWithVelocity);
    static void insertNotWallColliders(GameEngine& gameContext, std::vector<std::reference_wrapper<ColliderComponent>>& collidersNotWall);

    static bool objectiveInsideRange(SituationComponent& attackerSit, SituationComponent& objectiveSit, float rangeX, float rangeY);

    static bool checkCollidingWithObjective(BoundingBoxNode& boundingNode, int objId);
    static void deleteCollidingWithObjective(BoundingBoxNode& boundingNode, int objId);

    template <typename T> 
    static int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

private:

};