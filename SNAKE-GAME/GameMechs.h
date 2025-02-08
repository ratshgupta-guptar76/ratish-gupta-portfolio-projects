#ifndef GAMEMECHS_H
#define GAMEMECHS_H

#include <cstdlib>
#include <termios.h>
#include <ctime>
#include <iostream>
#include "objPos.h"
#include "objPosArrayList.h"


using namespace std;

class GameMechs
{
    private:
        char input;
        bool exitFlag;
        bool loseFlag;
        int score;

        int boardSizeX;
        int boardSizeY;

        objPos foodPos;

    public:
        GameMechs();
        GameMechs(int boardX, int boardY);
        ~GameMechs(); // is this one needed at all? Why or why not?
        
        bool getExitFlagStatus() const; 
        void setExitTrue();
        bool getLoseFlagStatus() const;
        void setLoseFlag();

        char getInput() const;
        void setInput(char this_input);
        void clearInput();

        int getBoardSizeX() const;
        int getBoardSizeY() const;
        
        int getScore() const;
        void incrementScore();
        
        // More methods should be added here
        void generateFood(objPosArrayList* playerPos);
        objPos getFoodElement() const;
        int getFoodSize() const;
};

#endif