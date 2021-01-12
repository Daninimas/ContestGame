#pragma once

#include "Component.hpp"
#include <tools/BoundingBox.hpp>
#include <tools/WorldPhase.hpp>
#include <tools/BackgroundLayer.hpp>
#include <tools/Sound.hpp>
#include <vector>
#include <string>

class WorldComponent : public Component {
public:
    explicit WorldComponent(const int id);
    ~WorldComponent() override;

    //std::vector<WorldPhase> worldPhases;
    //std::size_t currentPhase{ 0 };

    WorldPhase currentPhase;

    uint8_t currentPhaseNumber{ 1 }; // starts in 1
    uint8_t numberOfPhases;

    std::string worldPath;

    std::vector<BackgroundLayer> backgroundLayers;
};