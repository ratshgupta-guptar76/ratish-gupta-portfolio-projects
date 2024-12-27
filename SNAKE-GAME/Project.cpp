#include <iostream>
#include <windows.h>
#include "MacUILib.h"
#include "objPos.h"
#include "Player.h"
#include <conio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

#define DELAY_CONST 100000
#define MAX_SCORE 20
#define START_SIZE 10

// Global Objects
Player *playerPtr = nullptr;
GameMechs *gameMech = nullptr;

// Global Variables
int x;
int y;
int symb;
char input;
string buffer;

// Iterator Variables
int i;
int j;

// Constant Variables
int HEIGHT;
int WIDTH;
int OBJ_SIZE;
string Missing_Row = "";

// Function Prototypes
void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

int main(void)
{

    SetConsoleOutputCP(CP_UTF8);

    Initialize();

    while (gameMech->getExitFlagStatus() == false)
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();
    return 0;
}

void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    srand(time(NULL));

    // Allocating Heap Memory
    gameMech = new GameMechs(10, 20); // Game Mechanics Object
    playerPtr = new Player(gameMech, 1); // Start with only the head

    // Initialising Global Variables
    HEIGHT = gameMech->getBoardSizeX(); // Get Board Height
    WIDTH = gameMech->getBoardSizeY();   // Get Board Width
    symb = playerPtr->getPlayerPos()->getHeadElement().getSymbol();  // Get Player Symbol
    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); // Get Player Size

    buffer += u8"===============================\n\t SNAKE GAME \n===============================\n\n";

    // Generate Missing Row
    for (i = 0; i < WIDTH; i++)    
        (i == 0 || i == WIDTH - 1) ? Missing_Row += "░█░" : Missing_Row += "   ";

}

void GetInput(void)
{
    if (_kbhit()) {
        gameMech->setInput(_getch()); // Get Input
        // Increase snake size to 10 on first key press
        if (playerPtr->getPlayerPos()->getSize() == 1) {
            for (int i = 0; i < 9; i++) {
                playerPtr->getPlayerPos()->insertTail(objPos(playerPtr->getPlayerPos()->getTailElement().getX(), playerPtr->getPlayerPos()->getTailElement().getY(), symb));
            }
        }
    }
    else {
        gameMech->clearInput(); // Clear input if no input is given
    }
}

void RunLogic(void)
{
    input = gameMech->getInput();

    if (input == 32)
        gameMech->setExitTrue();

    playerPtr->updatePlayerDir();      // Update player direction
    playerPtr->movePlayer();    // Move player based on direction

    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); // Update Player Size

    if (gameMech->getScore() >= MAX_SCORE)
    {
        gameMech->setLoseFlag();
        gameMech->setExitTrue();
    }

    if (playerPtr->checkSelfCollision())
    {
        gameMech->setExitTrue();
    }

}

void DrawScreen(void)
{
    int food_x = gameMech->getFoodPos().getX();
    int food_y = gameMech->getFoodPos().getY();
    MacUILib_clearScreen();

    buffer.clear(); // Clear buffer at the start

    buffer += u8"===============================\n\t SNAKE GAME \n===============================\n\n";

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                buffer += u8"█░█";
            }
            else if (i == food_x && j == food_y)
            {
                buffer += u8" 🥚";
            }
            else
            {
                int k;
                for (k = 0; k < OBJ_SIZE; k++)
                {
                    if (i == playerPtr->getPlayerPos()->getElement(k).getX() && j == playerPtr->getPlayerPos()->getElement(k).getY())
                    {
                        if (k == 0)
                        {
                            buffer += " ";
                            buffer += "🐍";
                        }
                        else
                        {
                            buffer += u8" 🟩";
                        }
                        break;
                    }
                }
                if (k == OBJ_SIZE)
                {
                    buffer += "   ";
                }
            }
        }
        (i < HEIGHT - 1) ? buffer += "\n" + Missing_Row + "\n" : buffer += "\n\n";
    }

    printf("%s", buffer.c_str());
}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}

void CleanUp(void)
{
    MacUILib_clearScreen();


    // Display Win/Lose Message
    if (gameMech->getLoseFlagStatus()) 
        printf("                                                                       .-')      ('-.          \n                                                                      ( OO ).  _(  OO)             \n    ,--.   ,--..-'),-----.  ,--. ,--.          ,--.      .-'),-----. (_)---\\_)(,------.       \n     \\  `.'  /( OO'  .-.  ' |  | |  |          |  |.-') ( OO'  .-.  '/    _ |  |  .---'      \n   .-')     / /   |  | |  | |  | | .-')        |  | OO )/   |  | |  |\\  :` `.  |  |             \n  (OO  \\   /  \\_) |  |\\|  | |  |_|( OO )       |  |`-' |\\_) |  |\\|  | '..`''.)(|  '--.        \n   |   /  /\\_   \\ |  | |  | |  | | `-' /      (|  '---.'  \\ |  | |  |.-._)   \\ |  .--'      \n   `-./  /.__)   `'  '-'  '('  '-'(_.-'        |      |    `'  '-'  '\\       / |  `---.        \n     `--'          `-----'   `-----'           `------'      `-----'  `-----'  `------'        \n\n");
    else
        printf("\n     ____     __   ,-----.      ___    _         .--.      .--..-./`) ,---.   .--.          _ _  .--.     \n     \\   \\   /  /.'  .-,  '.  .'   |  | |        |  |_     |  |\\ .-.')|    \\  |  |         ( ` ) `-. \\    \n      \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        | _( )_   |  |/ `-' \\|  ,  \\ |  |        (_ o _)   \\_\\   \n       _( )_ .';  \\  '_ /  | :.'  '_  | |        |(_ o _)  |  | `-'`\"`|  |\\_ \\|  |         (_,_)   _( )_  \n   ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|        | (_,_) \\ |  | .---. |  _( )_\\  |                (_ o _) \n  |   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        |  |/    \\|  | |   | | (_ o _)  |           _     (_,_) \n  |   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)        |  '  /\\  `  | |   | |  (_,_)\\  |         _( )_    / /   \n   \\      /      '. \\_/``\".'   \\ /  . \\ /        |    /  \\    | |   | |  |    |  |        (_ o _).-' /    \n    `-..-'         '-----'      ``-'`-''         `---'    `---` '---' '--'    '--'         (_,_) `--'     \n                                                                                                          \n");
    
    // De-allocate Heap Memory
    delete playerPtr;
    delete gameMech;

    MacUILib_uninit();
}
