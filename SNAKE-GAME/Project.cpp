#include <iostream>
#include <windows.h>
#include "MacUILib.h"
#include "WinTerminal.h"
#include "objPos.h"
#include "Player.h"
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include <iomanip>

using namespace std;

#define DELAY_CONST 100000
#define BOARD_HEIGHT 50
#define BOARD_WIDTH 25
#define START_SIZE 10
#define MAX_SCORE (BOARD_HEIGHT-1) * (BOARD_WIDTH-1) - START_SIZE

#define EMPTY_CHAR u8" "
#define BORDER_CHAR_BLANK u8"███"
#define BORDER_CHAR_PATTERN u8"█░█"
#define BORDER_CHAR_PATTERN_2 u8"░█░"
#define SNAKE_HEAD_CHAR u8"🐍"
#define SNAKE_BODY_CHAR u8"-"
#define FOOD_CHAR u8"🥚"

// Global Objects
Player *playerPtr = nullptr;
GameMechs *gameMech = nullptr;

// Global Variables
int x;
int y;
string symb;
char input;
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

    // Black Text on White Background
    TERMINAL_COLOR(12, 0);

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
    gameMech = new GameMechs(BOARD_WIDTH, BOARD_HEIGHT); 
    playerPtr = new Player(gameMech, 1); 

    // // Set initial position of the snake
    // for (int i = 0; i < START_SIZE; i++) {
    //     playerPtr->getPlayerPos()->insertTail(objPos(BOARD_WIDTH / 2, BOARD_HEIGHT / 2 + i, SNAKE_BODY_CHAR));
    // }
    // playerPtr->getPlayerPos()->getHeadElement().setObjPos(BOARD_WIDTH / 2, BOARD_HEIGHT / 2, SNAKE_HEAD_CHAR);

    // Initialising Global Variables
    HEIGHT = gameMech->getBoardSizeX(); 
    WIDTH = gameMech->getBoardSizeY();   
    symb = playerPtr->getPlayerPos()->getHeadElement().getSymbol();  
    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); 

    prevHeadPos = new objPos();
    prevTailPos = new objPos();
}

void GetInput(void)
{
    if (_kbhit()) {
        gameMech->setInput(_getch()); // Get Input
        // Increase snake size to 10 on first key press
        if (gameMech->getInput() == 'w' || gameMech->getInput() == 'a' || gameMech->getInput() == 's' || gameMech->getInput() == 'd')
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
                    cout << BORDER_CHAR_BLANK;
                    TERMINAL_CURSOR_JUMP(j * 3, i * 2 + 1);
                    cout << BORDER_CHAR_BLANK;
                }
                else {
                    TERMINAL_CURSOR_JUMP(j * 3, i * 2);
                    cout << EMPTY_CHAR; 
                }
            }
        }
        firstRun = false;
    }

    TERMINAL_CURSOR_JUMP(prevTailPos->getY() * 3, prevTailPos->getX() * 2);
    cout << EMPTY_CHAR;

    TERMINAL_CURSOR_JUMP(0, 0);
    cout << setw(3) << setfill('0') << gameMech->getScore(); 

    for (int k = 0; k < OBJ_SIZE; k++)
    {
        if (k != 0)
        {
            
            TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getElement(k).getY() * 3, playerPtr->getPlayerPos()->getElement(k).getX() * 2);
            cout << playerPtr->getPlayerPos()->getElement(k).getSymbol();
        }
        else 
        {
            TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getElement(k).getY() * 3, playerPtr->getPlayerPos()->getElement(k).getX() * 2);
            cout << playerPtr->getPlayerPos()->getHeadElement().getSymbol();
        } 
        
            
    }

    TERMINAL_CURSOR_JUMP(gameMech->getFoodPos().getY() * 3, gameMech->getFoodPos().getX() * 2);
    cout << FOOD_CHAR;

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
        TERMINAL_COLOR(12, 0);
        TERMINAL_DELAY_SINGLE_LINE_printf("                                                                       .-')      ('-.          \n"      );
        TERMINAL_DELAY_SINGLE_LINE_printf("                                                                      ( OO ).  _(  OO)             \n"  );
        TERMINAL_DELAY_SINGLE_LINE_printf("    ,--.   ,--..-'),-----.  ,--. ,--.          ,--.      .-'),-----. (_)---\\_)(,------.       \n"      );
        TERMINAL_DELAY_SINGLE_LINE_printf("     \\  `.'  /( OO'  .-.  ' |  | |  |          |  |.-') ( OO'  .-.  '/    _ |  |  .---'      \n"       );
        TERMINAL_DELAY_SINGLE_LINE_printf("   .-')     / /   |  | |  | |  | | .-')        |  | OO )/   |  | |  |\\  :` `.  |  |             \n"    );
        TERMINAL_DELAY_SINGLE_LINE_printf("  (OO  \\   /  \\_) |  |\\|  | |  |_|( OO )       |  |`-' |\\_) |  |\\|  | '..`''.)(|  '--.        \n"  );
        TERMINAL_DELAY_SINGLE_LINE_printf("   |   /  /\\_   \\ |  | |  | |  | | `-' /      (|  '---.'  \\ |  | |  |.-._)   \\ |  .--'      \n"     );
        TERMINAL_DELAY_SINGLE_LINE_printf("   `-./  /.__)   `'  '-'  '('  '-'(_.-'        |      |    `'  '-'  '\\       / |  `---.        \n"     );
        TERMINAL_DELAY_SINGLE_LINE_printf("     `--'          `-----'   `-----'           `------'      `-----'  `-----'  `------'        \n\n"    );
    }
    else
    {
        TERMINAL_COLOR(10, 0);
        TERMINAL_DELAY_SINGLE_LINE_printf("\n     ____     __   ,-----.      ___    _         .--.      .--..-./`) ,---.   .--.          _ _  .--.     \n"         );
        TERMINAL_DELAY_SINGLE_LINE_printf("     \\   \\   /  /.'  .-,  '.  .'   |  | |        |  |_     |  |\\ .-.')|    \\  |  |         ( ` ) `-. \\    \n"      );
        TERMINAL_DELAY_SINGLE_LINE_printf("      \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        | _( )_   |  |/ `-' \\|  ,  \\ |  |        (_ o _)   \\_\\   \n"    );
        TERMINAL_DELAY_SINGLE_LINE_printf("       _( )_ .';  \\  '_ /  | :.'  '_  | |        |(_ o _)  |  | `-'`\"`|  |\\_ \\|  |         (_,_)   _( )_  \n"       );
        TERMINAL_DELAY_SINGLE_LINE_printf("   ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|        | (_,_) \\ |  | .---. |  _( )_\\  |                (_ o _) \n"       );
        TERMINAL_DELAY_SINGLE_LINE_printf("  |   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        |  |/    \\|  | |   | | (_ o _)  |           _     (_,_) \n"         );
        TERMINAL_DELAY_SINGLE_LINE_printf("  |   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)        |  '  /\\  `  | |   | |  (_,_)\\  |         _( )_    / /   \n"      );
        TERMINAL_DELAY_SINGLE_LINE_printf("   \\      /      '. \\_/``\".'   \\ /  . \\ /        |    /  \\    | |   | |  |    |  |        (_ o _).-' /    \n"     );
        TERMINAL_DELAY_SINGLE_LINE_printf("    `-..-'         '-----'      ``-'`-''         `---'    `---` '---' '--'    '--'         (_,_) `--'     \n"           );
    }

    TERMINAL_COLOR(15, 0);

    // De-allocate Heap Memory
    delete playerPtr;
    delete gameMech;

    MacUILib_uninit();
}
