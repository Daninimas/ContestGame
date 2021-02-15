#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AutodeleteSystem : public System {
public:
    AutodeleteSystem();
    ~AutodeleteSystem();

    void update(GameEngine& gameContext) const override;

private:

};

