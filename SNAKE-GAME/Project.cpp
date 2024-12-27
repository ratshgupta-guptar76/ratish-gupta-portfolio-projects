#include <iostream>
#include <windows.h>
#include "MacUILib.h"
#include "WinTerminal.h"
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
bool firstRun = true;
objPos* prevHeadPos;
objPos* prevTailPos;

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

    TERMINAL_CURSOR_HIDE();

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
    gameMech = new GameMechs(10, 20); 
    playerPtr = new Player(gameMech, 1); 

    // Initialising Global Variables
    HEIGHT = gameMech->getBoardSizeX(); 
    WIDTH = gameMech->getBoardSizeY();   
    symb = playerPtr->getPlayerPos()->getHeadElement().getSymbol();  
    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); 

    prevHeadPos = new objPos();
    prevTailPos = new objPos();

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
            objPos prevTail = playerPtr->getPlayerPos()->getTailElement(); // Store previous tail element
            for (int i = 0; i < 9; i++) {
                playerPtr->getPlayerPos()->insertTail(objPos(prevTail.getX(), prevTail.getY(), symb));
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

    playerPtr->updatePlayerDir();      
    playerPtr->movePlayer();    

    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); 

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
    objPos prevTail = playerPtr->getPlayerPos()->getTailElement(); // Define prevTail

    if (firstRun)
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                {
                    TERMINAL_CURSOR_JUMP(j * 3, i * 2);
                    cout << u8"█░█";
                    TERMINAL_CURSOR_JUMP(j * 3, i * 2 + 1);
                    cout << u8"░█░";
                }
            }
        }
        firstRun = false;
    }

    TERMINAL_CURSOR_JUMP(prevTailPos->getY() * 3, prevTailPos->getX() * 2);
    cout << "   ";
    TERMINAL_CURSOR_JUMP(prevHeadPos->getY() * 3, prevHeadPos->getX() * 2);
    cout << "🍑";

    for (int k = 0; k < OBJ_SIZE; k++)
    {
        if (k == 0)
        {
            TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getElement(k).getY() * 3, playerPtr->getPlayerPos()->getElement(k).getX() * 2);
            cout << " 🐍";
        }
        else 
        {
            TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getElement(k).getY() * 3, playerPtr->getPlayerPos()->getElement(k).getX() * 2);
            cout << u8" 🟩";
        }
    }

    TERMINAL_CURSOR_JUMP(gameMech->getFoodPos().getY() * 3, gameMech->getFoodPos().getX() * 2);
    cout << " 🥚";

    // Store Previous Head and Tail Positions
    *prevHeadPos = playerPtr->getPlayerPos()->getHeadElement();
    *prevTailPos = playerPtr->getPlayerPos()->getTailElement();
}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}

void CleanUp(void)
{
    TERMINAL_CLEAR();


    // Display Win/Lose Message
    if (gameMech->getLoseFlagStatus())
    {
        printf("                                                                       .-')      ('-.          \n"      );
        printf("                                                                      ( OO ).  _(  OO)             \n"  );
        printf("    ,--.   ,--..-'),-----.  ,--. ,--.          ,--.      .-'),-----. (_)---\\_)(,------.       \n"      );
        printf("     \\  `.'  /( OO'  .-.  ' |  | |  |          |  |.-') ( OO'  .-.  '/    _ |  |  .---'      \n"       );
        printf("   .-')     / /   |  | |  | |  | | .-')        |  | OO )/   |  | |  |\\  :` `.  |  |             \n"    );
        printf("  (OO  \\   /  \\_) |  |\\|  | |  |_|( OO )       |  |`-' |\\_) |  |\\|  | '..`''.)(|  '--.        \n"  );
        printf("   |   /  /\\_   \\ |  | |  | |  | | `-' /      (|  '---.'  \\ |  | |  |.-._)   \\ |  .--'      \n"     );
        printf("   `-./  /.__)   `'  '-'  '('  '-'(_.-'        |      |    `'  '-'  '\\       / |  `---.        \n"     );
        printf("     `--'          `-----'   `-----'           `------'      `-----'  `-----'  `------'        \n\n"    );
    }
    else
    {
        printf("\n     ____     __   ,-----.      ___    _         .--.      .--..-./`) ,---.   .--.          _ _  .--.     \n"         );
        printf("     \\   \\   /  /.'  .-,  '.  .'   |  | |        |  |_     |  |\\ .-.')|    \\  |  |         ( ` ) `-. \\    \n"      );
        printf("      \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        | _( )_   |  |/ `-' \\|  ,  \\ |  |        (_ o _)   \\_\\   \n"    );
        printf("       _( )_ .';  \\  '_ /  | :.'  '_  | |        |(_ o _)  |  | `-'`\"`|  |\\_ \\|  |         (_,_)   _( )_  \n"       );
        printf("   ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|        | (_,_) \\ |  | .---. |  _( )_\\  |                (_ o _) \n"       );
        printf("  |   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        |  |/    \\|  | |   | | (_ o _)  |           _     (_,_) \n"         );
        printf("  |   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)        |  '  /\\  `  | |   | |  (_,_)\\  |         _( )_    / /   \n"      );
        printf("   \\      /      '. \\_/``\".'   \\ /  . \\ /        |    /  \\    | |   | |  |    |  |        (_ o _).-' /    \n"     );
        printf("    `-..-'         '-----'      ``-'`-''         `---'    `---` '---' '--'    '--'         (_,_) `--'     \n"           );
    }
    // De-allocate Heap Memory
    delete playerPtr;
    delete gameMech;

    MacUILib_uninit();
}
