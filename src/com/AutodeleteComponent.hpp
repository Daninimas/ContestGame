#pragma once

#include "Component.hpp"
#include <enum/GameObjectType.hpp>

class AutodeleteComponent : public Component {
public:
    explicit AutodeleteComponent(const int id);  // Component that makes delete this entity on a certain time
    ~AutodeleteComponent() override;

    float timeToDelete{ 1.f };
    float timeToDeleteCounter{ 0.f };
};
