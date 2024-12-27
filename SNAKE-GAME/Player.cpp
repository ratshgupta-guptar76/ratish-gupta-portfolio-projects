#include "Player.h"
#include "objPosArrayList.h"

Player::Player(GameMechs *thisGMRef, int initialLength)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    // Initialize playerPos
    playerPos = new objPosArrayList();

    // Initialize player position to mimic snake body
    int initialX = 5;
    int initialY = 5;

    for (int i = 0; i < initialLength; ++i) {
        objPos initialPos(initialX, initialY - i, '*');
        playerPos->insertHead(initialPos);
    }
}

Player::~Player()
{
    // Clean up playerPos
    delete playerPos;
}

objPosArrayList* Player::getPlayerPos() const
{
    return playerPos;
}

bool Player::checkFoodConsumption()
{
    if (playerPos->getSize() == 0) return false;

    objPos head = playerPos->getHeadElement();
    objPos food = mainGameMechsRef->getFoodPos();

    return food.getX() == head.getX() && food.getY() == head.getY();
}

bool Player::checkSelfCollision()
{
    if (myDir == STOP || playerPos->getSize() == 0) return false;
    
    objPos head = playerPos->getHeadElement();

    for (int i = 1; i < playerPos->getSize(); i++)
    {
        if (head.getX() == playerPos->getElement(i).getX() && head.getY() == playerPos->getElement(i).getY())
            return true;
    }

    return false;
}

void Player::updatePlayerDir()
{
    // PPA3 input processing logic
    char input = mainGameMechsRef->getInput();

    switch (input)
    {
    case 'w':
        if (myDir != DOWN)
            myDir = UP;
        break;

    case 'a':
        if (myDir != RIGHT)
            myDir = LEFT;
        break;

    case 's':
        if (myDir != UP)
            myDir = DOWN;
        break;

    case 'd':
        if (myDir != LEFT)
            myDir = RIGHT;
        break;

    default:
        break;
    }
}

void Player::movePlayer()
{
    if (playerPos->getSize() == 0) return;

    // PPA3 Finite State Machine logic
    int xPos = playerPos->getHeadElement().getX();
    int yPos = playerPos->getHeadElement().getY();

    // Head Symbol
    char headSymbol;

    switch (myDir)
    {
    case UP:
        xPos -= 1;
        headSymbol = '^';
        break;

    case RIGHT:
        yPos += 1;
        headSymbol = '>';
        break;

    case DOWN:
        xPos += 1;
        headSymbol = 'v';
        break;

    case LEFT:
        yPos -= 1;
        headSymbol = '<';
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

// More methods to be added