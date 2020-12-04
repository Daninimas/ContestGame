#pragma once

#include "Component.hpp"
#include <enum/TriggerFunction.hpp>
#include <enum/EntityType.hpp>
#include <enum/GameObjectType.hpp>
#include <tools/Sound.hpp>
#include <vector>

class TriggerComponent : public Component {
public:
    explicit TriggerComponent(const int id);
    ~TriggerComponent() override;

    std::vector<TriggerFunction> functions;

    // For the music and sound functions
    Sound sound;

    // For the entity generation
    /*EntityType entityType;
    GameObjectType gameObjectType;*/
};