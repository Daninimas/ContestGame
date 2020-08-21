#pragma once

#include "Component.hpp"

class SituationComponent : public Component {
public:
    explicit SituationComponent(const int id);
    ~SituationComponent() override;

    float	x			{0.f};
    float	y			{0.f};
    float	rotation	{0.f};
};
