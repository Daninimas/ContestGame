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


private:

};