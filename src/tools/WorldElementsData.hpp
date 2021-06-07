#pragma once

#include <tools/BoundingBox.hpp>
#include <tools/Sound.hpp>
#include <limits>

class WorldElementsData {
public:
    WorldElementsData() {};
    ~WorldElementsData() = default;

    static inline int worldId = 0;
    static inline int playerId = std::numeric_limits<int>::max();
    static inline int activeCameraId = 0;
    static inline int playerDroneId = std::numeric_limits<int>::max();
    static inline std::vector<int> worldDistanceWeapons{};
    static inline std::vector<int> worldMeleeWeapons{};
    static inline uint16_t enemiesInWorld{ 0 };

    // Score of the player
    static inline int playerScore{ 0 };

    // Counter for wait after lose Live
    static inline float waitLoseLifeTime{ 1.f };
    static inline float waitLoseLifeTimeCounter{ 0.f };

    // Time spended in the phase
    static inline float timeInPhase{ 0.f }; // Con este tiempo por cada fase vamos sumando puntuacion
};