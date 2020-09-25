#pragma once

#include <tools/BoundingBox.hpp>

class WorldData {
public:
    WorldData() {};
    ~WorldData() = default;

    static inline BoundingBox worldLimits = {0.f, 0.f, 0.f, 0.f};
    static inline int playerId = 0;
    static inline std::vector<int> worldDistanceWeapons{};
    static inline std::vector<int> worldMeleeWeapons{};
};