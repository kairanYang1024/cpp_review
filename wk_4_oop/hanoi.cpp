//tower of hanoi: programmatically w/ code
//cubes: uiuc.cube w/ cube
//game: the frame
//stack: each stack has a vector of cubes
//interaction to the top of the stack --> only the top is accessible
#include <iostream>
#include "Game.h"

int main() {
    Game g;
    std::cout << "Initial game state: " << std::endl;
    std::cout << g << std::endl;

    g.solve();

    std::cout << "Final game state: " << std::endl;
    std::cout << g << std::endl;
}