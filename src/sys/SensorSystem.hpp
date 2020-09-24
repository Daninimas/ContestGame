#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>
#include <tools/BoundingBox.hpp>

class SensorSystem : public System {
public:
    SensorSystem();
    ~SensorSystem();

    void update(GameEngine& gameContext) const override;

private:
    void resetSensors(GameEngine& gameContext) const;
    void checkSensorsCollisions(GameEngine& gameContext) const;
    bool calculateSensorCollision(GameEngine& gameContext, SensorComponent& sensor, BoundingBox& entityBounding, SituationComponent& entitySituation) const;
    BoundingBox getSensorBoundingDependingFacing(GameEngine& gameContext, SensorComponent& sensor) const;
};

