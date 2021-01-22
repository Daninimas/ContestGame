#pragma once

#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>
#include <limits>

class WorldElementsData {
public:
    WorldElementsData() {};
    ~WorldElementsData() = default;

    static inline int worldId = 0;
    static inline int playerId = 0;
    static inline int activeCameraId = 0;
    static inline int playerDroneId = std::numeric_limits<int>::max();
    static inline std::vector<int> worldDistanceWeapons{};
    static inline std::vector<int> worldMeleeWeapons{};
    static inline uint16_t enemiesInWorld{ 0 };

    // Counter for wait after lose Live
    static inline float waitLoseLifeTime{ 1.f };
    static inline float waitLoseLifeTimeCounter{ 0.f };
};