#include "Game.h"

using std::cout;
using std::endl;

const int STACK_SIZE = 3;
Game::Game() {
    for(int i = 0; i < STACK_SIZE; i++) {
        Stack stackOfCubes; //init
        stacks_.push_back(stackOfCubes);
    }

    //descending order of length
    Cube blue(4, uiuc::HSLAPixel::BLUE);
    stacks_[0].push_back(blue);
    Cube orange(3, uiuc::HSLAPixel::ORANGE);
    stacks_[0].push_back(orange);
    Cube purple(2, uiuc::HSLAPixel::PURPLE);
    stacks_[0].push_back(purple);
    Cube yellow(1, uiuc::HSLAPixel::YELLOW);
    stacks_[0].push_back(yellow);
}

void Game::solve() {
    // Prints out the state of the game:
    std::cout << *this << std::endl;
    // using recursive plan
    //top k-1 subset: stack 1, bottom: stack 2
    //swap: old 'spare' is now 'target' -- rewind
    //move(Source[start:end] -> Target) =
    //move(Src[(start+1):end] -> Spare)
    //move(Src[start] -> Target)
    //move(Spare[(start+1):end] -> Target)
     _move(
      0, stacks_[0].size() - 1,  //< Move the entire set of cubes, [0 .. size-1]
      stacks_[0], //< Source stack is [0]
      stacks_[2], //< Target stack is [2]
      stacks_[1], //< Spare stack is [1]
      0   //< Initial depth (for printouts only) is 0
    );
}

//moving cube directly from a stack to another
void Game::_moveCube(Stack & s1, Stack & s2) {
  Cube cube = s1.removeTop();
  s2.push_back(cube);
}

//recursively moving cubes[start:end] with 'depth' in callstack
void Game::_move(unsigned start, unsigned end, Stack& source, Stack& target, 
        Stack& spare, unsigned depth) {
                              //depth-increment
    cout << "Planning (depth=" << depth++ << "): Move [" << start << ".." << end << "] from Stack@" << &source << " -> Stack@" << &target << ", Spare@" << &spare << "]" << endl;
    if(start == end) {
      _moveCube(source, target);
      cout << *this << endl;
    } else {
      _move(start+1, end, source, spare, target, depth); //swapping here
      _move(start, start, source, target, spare, depth);
      _move(start+1, end, spare, target, source, depth); //backward transfer
    }
}
std::ostream& operator<<(std::ostream & os, const Game & game) {
    for (unsigned i = 0; i < game.stacks_.size(); i++) {
      os << "Stack[" << i << "]: " << game.stacks_[i];
    }
    return os;
}