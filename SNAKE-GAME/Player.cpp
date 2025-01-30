#include "Player.h"
#include "objPosArrayList.h"

#define BODY_SYMBOL u8"🟦"
#define HEAD_SYMBOL_UP u8"🐍"
#define HEAD_SYMBOL_DOWN u8"🐍"
#define HEAD_SYMBOL_LEFT u8"🐍"
#define HEAD_SYMBOL_RIGHT u8"🐍"

Player::Player(GameMechs *thisGMRef, int initialLength)
{
    mainGameMechsRef = thisGMRef;
    myDir = Direction::STOP;

    // Initialize playerPos
    playerPos = new objPosArrayList();

    // Initialize player position to mimic snake body
    int initialX = rand() % (mainGameMechsRef->getBoardSizeX() - 2) + 1;
    int initialY = rand() % (mainGameMechsRef->getBoardSizeY() - 2) + 1;

    for (int i = 0; i < initialLength; ++i)
    {
        objPos initialPos(initialX, initialY - i, BODY_SYMBOL);
        playerPos->insertHead(initialPos);
    }
}

Player::~Player()
{
    // Clean up playerPos
    delete playerPos;
}

objPosArrayList *Player::getPlayerPos() const
{
    return playerPos;
}

bool Player::checkFoodConsumption()
{
    if (playerPos->getSize() == 0)
        return false;

    objPos head = playerPos->getHeadElement();
    objPos food = mainGameMechsRef->getFoodElement();

    return food.getX() == head.getX() && food.getY() == head.getY();
}

bool Player::checkSelfCollision()
{
    if (myDir == Direction::STOP || playerPos->getSize() == 0)
        return false;

    objPos head = playerPos->getHeadElement();

    for (int i = 1; i < playerPos->getSize(); i++)
    {
        if (head.getX() == playerPos->getElement(i).getX() && head.getY() == playerPos->getElement(i).getY())
            return true;
    }

    return false;
}

Direction::DIR Player::getDir() const
{
    return myDir;
}

void Player::updatePlayerDir()
{
    // PPA3 input processing logic
    char input = mainGameMechsRef->getInput();

    switch (input)
    {
    case 'w':
        if (myDir != Direction::DOWN)
            myDir = Direction::UP;
        break;

    case 'a':
        if (myDir != Direction::RIGHT)
            myDir = Direction::LEFT;
        break;

    case 's':
        if (myDir != Direction::UP)
            myDir = Direction::DOWN;
        break;

    case 'd':
        if (myDir != Direction::LEFT)
            myDir = Direction::RIGHT;
        break;

    default:
        break;
    }
}

void Player::movePlayer()
{
    if (playerPos->getSize() == 0)
        return;

    // PPA3 Finite State Machine logic
    int xPos = playerPos->getHeadElement().getX();
    int yPos = playerPos->getHeadElement().getY();

    // Head Symbol
    string headSymbol;

    switch (myDir)
    {
    case Direction::UP:
        xPos -= 1;
        headSymbol = HEAD_SYMBOL_UP;
        break;

    case Direction::RIGHT:
        yPos += 1;
        headSymbol = HEAD_SYMBOL_RIGHT;
        break;

    case Direction::DOWN:
        xPos += 1;
        headSymbol = HEAD_SYMBOL_DOWN;
        break;

    case Direction::LEFT:
        yPos -= 1;
        headSymbol = HEAD_SYMBOL_LEFT;
        break;

    case Direction::STOP:
        break;

    default:
        break;
    }

    // Border wraparound
    if (xPos < 1)
    {
        xPos = mainGameMechsRef->getBoardSizeX() - 2;
    }
    else if (xPos >= mainGameMechsRef->getBoardSizeX() - 1)
    {
        xPos = 1;
    }

    if (yPos < 1)
    {
        yPos = mainGameMechsRef->getBoardSizeY() - 2;
    }
    else if (yPos >= mainGameMechsRef->getBoardSizeY() - 1)
    {
        yPos = 1;
    }

    playerPos->makeHeadBody(BODY_SYMBOL);

    objPos newHead;
    newHead.setObjPos(xPos, yPos, headSymbol);

    playerPos->insertHead(newHead);

    if (checkFoodConsumption())
    {
        mainGameMechsRef->generateFood(playerPos);
        mainGameMechsRef->incrementScore();
    }

    else
        playerPos->removeTail();
}