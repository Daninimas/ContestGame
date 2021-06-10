#pragma once

#include <eng/GameEngine.hpp>
#include <tools/BoundingBox.hpp>
#include <tools/Vector2.hpp>
#include <string>


class Utils {
public:
    Utils();
    static float stringToFloat(std::string string);
    static int stringToInt(std::string string);

    static float    calculateDistance(const float posX1, const float posX2, const float posY1, const float posY2);
    static float    degToRad(float angle);
    static float    radToDeg(float angle);

    static float getVectorMagnitude(Vector2& vec);
    static Vector2 rotateVector(Vector2& vect, float angle); // Gets angle in DEG

    static BoundingBox moveToWorldCoords(BoundingBox& bounding, SituationComponent& situation);

    static void insertCollidersIdWithVelocity(GameEngine& gameContext, std::vector<int>& idCollidersWithVelocity);
    static void insertNotWallColliders(GameEngine& gameContext, std::vector<std::reference_wrapper<ColliderComponent>>& collidersNotWall);

    static bool objectiveInsideRange(SituationComponent& attackerSit, SituationComponent& objectiveSit, const Vector2 range);

    static bool isEntitySensoredBy(SensorComponent& sensor, const int sensoredEntity);

    static bool checkCollidingWithEntity(BoundingBoxNode& boundingNode, int objId);
    static void deleteCollidingWithObjective(BoundingBoxNode& boundingNode, int objId);

    static void setNormalPistolToEntity(GameEngine& gameContext, int entityId);

    static Vector2 getCenterOfBounding(BoundingBox const bounding);

    static void printBoundingBox(BoundingBox const bounding);
    
    static BoundingBox getCameraViewBoundig(CameraComponent& cameraComp);

    static bool entityInPhaseLimit(GameEngine& gameContext, SituationComponent& entitySit);

    static void setPhaseStartToView(GameEngine& gameContext, uint8_t lastPhaseDirection);

    static void setControlKeyToMenuOptions(GameEngine& gameContext, MenuComponent& controlsMenu);

    static std::string getKeyName(uint8_t keyCode);
    
    static void resetPlayerPosition(GameEngine& gameContext);

    static SituationComponent* getClosestWallXToObjetive(GameEngine& gameContext, SituationComponent& objetiveSituation, bool onlyWALLGameObject = false); // The bool is for not picking a platform
    static SituationComponent* getClosestCheckpointXToObjetive(GameEngine& gameContext, SituationComponent& objetiveSituation); // The bool is for not picking a platform

    template <typename T> 
    static int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    static void updateControlsJSON(InputComponent& inputComp);

    static float normalizeValues(float minNormalized, float maxNormalized, float minRange, float maxRange, float value);

    static int getNewPlayerScorePosition(); // Devuelve la posicion en la que est� esta nueva puntuacion (-1 si no entra en la lista)

private:

    static inline const std::map<uint8_t, std::string> keyNameMap {
        { 0, "A" },
        { 1, "B" },            ///< The B key
        { 2, "C" },            ///< The C key
        { 3, "D" },            ///< The D key
        { 4, "E" },            ///< The E key
        { 5, "F" },            ///< The F key
        { 6, "G" },            ///< The G key
        { 7, "H" },            ///< The H key
        { 8, "I" },            ///< The I key
        { 9, "J" },            ///< The J key
        { 10, "K" },            ///< The K key
        { 11, "L" },            ///< The L key
        { 12, "M" },            ///< The M key
        { 13, "N" },            ///< The N key
        { 14, "O" },            ///< The O key
        { 15, "P" },            ///< The P key
        { 16, "Q" },            ///< The Q key
        { 17, "R" },            ///< The R key
        { 18, "S" },            ///< The S key
        { 19, "T" },            ///< The T key
        { 20, "U" },            ///< The U key
        { 21, "V" },            ///< The V key
        { 22, "W" },            ///< The W key
        { 23, "X" },            ///< The X key
        { 24, "Y" },            ///< The Y key
        { 25, "Z" },            ///< The Z key
        { 26, "Num0" },         ///< The 0 key
        { 27, "Num1" },         ///< The 1 key
        { 28, "Num2" },         ///< The 2 key
        { 29, "Num3" },         ///< The 3 key
        { 30, "Num4" },         ///< The 4 key
        { 31, "Num5" },         ///< The 5 key
        { 32, "Num6" },         ///< The 6 key
        { 33, "Num7" },         ///< The 7 key
        { 34, "Num8" },         ///< The 8 key
        { 35, "Num9" },         ///< The 9 key
        { 36, "Escape" },       ///< The Escape key
        { 37, "LControl" },     ///< The left Control key
        { 38, "LShift" },       ///< The left Shift key
        { 39, "LAlt" },         ///< The left Alt key
        { 40, "LSystem" },      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        { 41, "RControl" },     ///< The right Control key
        { 42, "RShift" },       ///< The right Shift key
        { 43, "RAlt" },         ///< The right Alt key
        { 44, "RSystem" },      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        { 45, "Menu" },         ///< The Menu key
        { 46, "LBracket" },     ///< The [ key
        { 47, "RBracket" },     ///< The ] key
        { 48, "Semicolon" },    ///< The ; key
        { 49, "Comma" },        ///< The , key
        { 50, "Period" },       ///< The . key
        { 51, "Quote" },        ///< The ' key
        { 52, "Slash" },        ///< The / key
        { 53, "Backslash" },    ///< The \ key
        { 54, "Tilde" },        ///< The ~ key
        { 55, "Equal" },        ///< The = key
        { 56, "Hyphen" },       ///< The - key (hyphen)
        { 57, "Space" },        ///< The Space key
        { 58, "Enter" },        ///< The Enter/Return keys
        { 59, "Backspace" },    ///< The Backspace key
        { 60, "Tab" },          ///< The Tabulation key
        { 61, "PageUp" },       ///< The Page up key
        { 62, "PageDown" },     ///< The Page down key
        { 63, "End" },          ///< The End key
        { 64, "Home" },         ///< The Home key
        { 65, "Insert" },       ///< The Insert key
        { 66, "Delete" },       ///< The Delete key
        { 67, "Add" },          ///< The + key
        { 68, "Subtract" },     ///< The - key (minus, usually from numpad)
        { 69, "Multiply" },     ///< The * key
        { 70, "Divide" },       ///< The / key
        { 71, "Left" },         ///< Left arrow
        { 72, "Right" },        ///< Right arrow
        { 73, "Up" },           ///< Up arrow
        { 74, "Down" },         ///< Down arrow
        { 75, "Numpad0" },      ///< The numpad 0 key
        { 76, "Numpad1" },      ///< The numpad 1 key
        { 77, "Numpad2" },      ///< The numpad 2 key
        { 78, "Numpad3" },      ///< The numpad 3 key
        { 79, "Numpad4" },      ///< The numpad 4 key
        { 80, "Numpad5" },      ///< The numpad 5 key
        { 81, "Numpad6" },      ///< The numpad 6 key
        { 82, "Numpad7" },      ///< The numpad 7 key
        { 83, "Numpad8" },      ///< The numpad 8 key
        { 84, "Numpad9" },      ///< The numpad 9 key
        { 85, "F1" },           ///< The F1 key
        { 86, "F2" },           ///< The F2 key
        { 87, "F3" },           ///< The F3 key
        { 88, "F4" },           ///< The F4 key
        { 89, "F5" },           ///< The F5 key
        { 90, "F6" },           ///< The F6 key
        { 91, "F7" },           ///< The F7 key
        { 92, "F8" },           ///< The F8 key
        { 93, "F9" },           ///< The F9 key
        { 94, "F10" },          ///< The F10 key
        { 95, "F11" },          ///< The F11 key
        { 96, "F12" },          ///< The F12 key
        { 97, "F13" },          ///< The F13 key
        { 98, "F14" },          ///< The F14 key
        { 99, "F15" },          ///< The F15 key
        { 100, "Pause" }        ///< The Pause key
    };

};