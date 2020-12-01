#pragma once

#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>

class WorldElementsData {
public:
    WorldElementsData() {};
    ~WorldElementsData() = default;

    static inline int worldId = 0;
    static inline int playerId = 0;
    static inline int activeCameraId = 0;
    static inline std::vector<int> worldDistanceWeapons{};
    static inline std::vector<int> worldMeleeWeapons{};
    static inline uint16_t enemiesInWorld{ 0 };
};