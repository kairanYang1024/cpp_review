#pragma once

#include <vector>
#include "Stack.h"
#include <iostream>
using std::vector;
using uiuc::Cube;

class Game {
    public:
        Game();
        void solve();
        friend std::ostream& operator<<(std::ostream& os, const Game& game);

    private:
        vector<Stack> stacks_;
        void _moveCube(Stack & s1, Stack & s2);
        void _move(unsigned start, unsigned end, Stack& source, Stack& target, 
        Stack& spare, unsigned depth);

};