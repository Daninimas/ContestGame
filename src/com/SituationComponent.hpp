#pragma once

#include <tools/Vector2.hpp>
#include "Component.hpp"
#include <limits>
#include <vector>

class SituationComponent : public Component {
public:
    explicit SituationComponent(const int id);
    ~SituationComponent() override;

    Vector2 position    { 0.f, 0.f };
    float	rotation	{0.f};

    Vector2	scale      { 1.f, 1.f };

    enum {
        Left,
        Right
    };
    uint8_t facing{ Right };

    bool noWorldDelete{ false }; // to prevent deleting this entity hen it is outside of the world


    // Para hacer entidades con hijos y padre
    int father = std::numeric_limits<int>::max();
    std::vector<int> sons{}; // Hay que tener en cuenta que los hijos tienen la posicion relativa al padre, no al mundo
};
