#pragma once

#include "Component.hpp"
#include <tools/BoundingBox.hpp>

#include <vector>

class SensorComponent : public Component {
public:
    explicit SensorComponent(const int id);
    ~SensorComponent() override;

    std::vector<int> entitiesSensoring;

    BoundingBox sensorBounding{ 0.f, 0.f, 0.f, 0.f };
};

