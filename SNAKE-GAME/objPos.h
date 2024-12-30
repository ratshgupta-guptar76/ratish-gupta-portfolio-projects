#ifndef OBJPOS_H
#define OBJPOS_H

#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

// Not really a C++ thing
typedef struct 
{
    int x;
    int y;
} Pos;

class objPos
{
    private:
        Pos* pos;
        string symbol;

    public:
        objPos();
        objPos(int xPos, int yPos, string sym);
        objPos(const objPos &o);
        objPos& operator = (const objPos &o);
        
        // Respect the rule of six / minimum four
        // [TODO] Implement the missing special member functions to meet the minimum four rule
        ~objPos();
        
        void setObjPos(objPos o);
        void setObjPos(int xPos, int yPos, string sym);

        objPos getObjPos() const;
        string getSymbol() const;
        int getX(); // Get Row Number/x-value
        int getY(); // Get Column Number/y-value
        string getSymbolIfPosEqual(const objPos* refPos) const;
        
        bool isPosEqual(const objPos* refPos) const;
};

#endif