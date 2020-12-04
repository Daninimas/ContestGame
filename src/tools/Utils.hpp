#pragma once

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Vector2.hpp>
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

    static bool objectiveInsideRange(SituationComponent& attackerSit, SituationComponent& objectiveSit, const Vector2 range);

    static bool checkCollidingWithEntity(BoundingBoxNode& boundingNode, int objId);
    static void deleteCollidingWithObjective(BoundingBoxNode& boundingNode, int objId);

    static void setNormalPistolToEntity(GameEngine& gameContext, int entityId);

    static Vector2 getCenterOfBounding(BoundingBox const bounding);

    static void printBoundingBox(BoundingBox const bounding);
    
    static BoundingBox getCameraViewBoundig(CameraComponent& cameraComp);

    static bool entityInPhaseLimit(GameEngine& gameContext, SituationComponent& entitySit);

    static void setPhaseStartToView(GameEngine& gameContext);

    template <typename T> 
    static int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

private:

};