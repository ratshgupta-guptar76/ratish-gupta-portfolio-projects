#include "GameMechs.h"
#include <ctime>
#include <iostream>
// ###########################################################################################
//      Object Completed by Ratish - Used for debugging Player class
// ###########################################################################################


// Method completed by Ratish - Used for debugging Player class
GameMechs::GameMechs()
    : boardSizeX(15), boardSizeY(30), input(0), score(0), exitFlag(false), loseFlag(true)
{
    foodPos = new objPos[6];

    foodPos[0].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[1].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[2].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[3].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[4].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[5].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");

}

// Method completed by Ratish - Used for debugging Player class
GameMechs::GameMechs(int boardX, int boardY)
    : boardSizeX(boardX), boardSizeY(boardY), input(0), score(0), exitFlag(false), loseFlag(true)
{
    foodPos = new objPos[6];

    foodPos[0].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[1].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[2].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[3].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[4].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
    foodPos[5].setObjPos(rand() % (boardSizeX - 2) + 1, rand() % (boardSizeY - 2) + 1, "@");
}

// do you need a destructor? 
GameMechs::~GameMechs()
{
    delete foodPos;
}

bool GameMechs::getExitFlagStatus() const
{
    return exitFlag;
}

bool GameMechs::getLoseFlagStatus() const
{
    return loseFlag;
}
    

char GameMechs::getInput() const
{
    return input;
}

int GameMechs::getScore() const
{
    return score;
}

void GameMechs::incrementScore()
{
    score+=1;
}

int GameMechs::getBoardSizeX() const
{
    return boardSizeX;
}

int GameMechs::getBoardSizeY() const
{
    return boardSizeY;
}

void GameMechs::setExitTrue()
{
    exitFlag = true;
}

void GameMechs::setLoseFlag()
{
    loseFlag = false;
}

void GameMechs::setInput(char this_input)
{
    input = this_input;
}

// Method completed by Ratish - Used for debugging Player class
void GameMechs::clearInput()
{
    input = 0;
}

// More methods should be added here
void GameMechs::generateFood(objPosArrayList* playerPos)
{
    int xRange = getBoardSizeX();
    int yRange = getBoardSizeY();
    int x, y;
    int playerSize = playerPos->getSize();

    bool valid = false;
    while (!valid) {
        x = rand() % (xRange - 2) + 1;
        y = rand() % (yRange - 2) + 1;
        valid = true;

        for (int j = 0; j < playerSize; j++) {
            if (x == playerPos->getElement(j).getX() && y == playerPos->getElement(j).getY()) {
                valid = false;
                break;
            }
        }
    }

    foodPos->setObjPos(x, y, "@");
}

objPos GameMechs::getFoodPos() const
{
    return foodPos->getObjPos();
}

int GameMechs::getFoodSize() const
{
    return 6;
}
            // for (int j = 0; j < i; j++) {
            //     for (int k = 0; k < playerSize; k++){
            //         if (list-i-x == list-j-x && list-i-y == list-j-y) {
            //             valid = 0;
            //             break;
            //         }
            //     }
            // }