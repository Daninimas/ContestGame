#include <eng/GameEngine.hpp>

int main () {

    GameEngine gameEngine{};
    gameEngine.init();
    gameEngine.run();

    return 0;
}