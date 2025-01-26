#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "GameMechs.h"
#include "objPos.h"
#include "objPosArrayList.h"

using namespace std;

namespace Direction
{
    enum DIR
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        STOP
    };
} // namespace Dir

class Player
{
    // Construct the remaining declaration from the project manual.

    // Only some sample members are included here

    // You will include more data members and member functions to complete your design.

public:
    Player(GameMechs *thisGMRef);
    Player(GameMechs *thisGMRef, int initialLength); // Updated constructor
    ~Player();

    objPosArrayList *getPlayerPos() const; // Upgrade this in iteration 3.
    Direction::DIR getDir() const;
    void updatePlayerDir();
    void movePlayer();

    // More methods to be added here
    bool checkFoodConsumption();
    bool checkSelfCollision();

private:
    objPosArrayList *playerPos; // Upgrade this in iteration 3.
    Direction::DIR myDir;

    // Need a reference to the Main Game Mechanisms
    GameMechs *mainGameMechsRef;
};

#endif