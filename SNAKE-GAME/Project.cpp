#include "MacUILib.h"
#include "Terminal.h"
#include "objPos.h"
#include "Player.h"
#include <locale.h>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>

// Make it so that every 10 steps reduces 1 point, and 1 snake length
// Graph Vortex

using namespace std;

#define DELAY_CONST 100000 // Delay of 1000 microseconds (1 milisecond)
#define BOARD_HEIGHT 50
#define BOARD_WIDTH 20
#define START_SIZE 10
#define MAX_SCORE 10

#define SPACE_CHAR u8"   "
#define EMPTY_CHAR u8" • "
#define BORDER_CHAR_BLANK u8"███"
#define BORDER_CHAR_PATTERN u8"█░█"
#define BORDER_CHAR_PATTERN_2 u8"░█░"
#define SNAKE_HEAD_CHAR u8"🐍"

// Global Objects
Player *playerPtr = nullptr;
GameMechs *gameMech = nullptr;

// Global Variables
int x;
int y;
int digits;
string hsymb;
string bsymb;
char input;
bool firstRun = true;
objPos *prevHeadPos;
objPos *prevTailPos;
objPos *prevFoodPos;

// Iterator Variables
int i;
int j;

// Constant Variables
int HEIGHT;
int WIDTH;
int OBJ_SIZE;

// Time Variables
chrono::time_point<chrono::high_resolution_clock> startTime, endTime, logicRunTime, printRunTime; // Create two time points
chrono::duration<float, milli> del_t, logic_t, print_t;

// Dummy Functions
int CountDigits(int num)
{
    int count = 0;
    while (num > 0)
    {
        count++;
        num = num / 10;
    }
    return count;
}

// Function Prototypes
void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

int main(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");

    TERMINAL_CURSOR_HIDE();

    // Red Text on Black Background
    TERMINAL_COLOR(13, 0);

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
    system("cls");

    srand(time(NULL));

    // Allocating Heap Memory
    gameMech = new GameMechs(BOARD_WIDTH, BOARD_HEIGHT);
    playerPtr = new Player(gameMech, 1); // Start only as a single snake element i.e. snake head

    // Initialising Global Variables
    HEIGHT = gameMech->getBoardSizeX();
    WIDTH = gameMech->getBoardSizeY();
    hsymb = playerPtr->getPlayerPos()->getHeadElement().getSymbol();
    bsymb = (new Player(gameMech, 2))->getPlayerPos()->getTailElement().getSymbol();
    OBJ_SIZE = playerPtr->getPlayerPos()->getSize();

    prevHeadPos = new objPos();
    prevTailPos = new objPos();
    prevFoodPos = new objPos();

    int digits = CountDigits(MAX_SCORE);
}

void GetInput(void)
{
    if (TERMINAL_CHECK_INPUT())
    {
        gameMech->setInput(TERMINAL_GET_INPUT()); // Get Input
        // Increase snake size to 10 on first key press
        if (gameMech->getInput() == 'w' || gameMech->getInput() == 'a' || gameMech->getInput() == 's' || gameMech->getInput() == 'd')
            if (playerPtr->getPlayerPos()->getSize() == 1)
            {
                objPos prevTail = playerPtr->getPlayerPos()->getTailElement(); // Store previous tail element
                for (int i = 0; i < 9; i++)
                {
                    playerPtr->getPlayerPos()->insertTail(objPos(prevTail.getX(), prevTail.getY(), bsymb));
                }
            }
    }
    else
    {
        gameMech->clearInput(); // Clear input if no input is given
    }
}

void RunLogic(void)
{
    startTime = chrono::high_resolution_clock::now();
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

    logicRunTime = chrono::high_resolution_clock::now();
}

// ================================================================================
//  ============================= DRAW SCREEN LOGIC ==============================
// ================================================================================
void first_print(void)
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
            else
            {
                TERMINAL_CURSOR_JUMP(j * 3, i * 2);
                cout << EMPTY_CHAR;
            }
        }
    }

    // Print snake head
    TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getHeadElement().getY() * 3, playerPtr->getPlayerPos()->getHeadElement().getX() * 2);
    cout << SNAKE_HEAD_CHAR;

    // Only This prints the snakehead initially for one iteration
    // TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getHeadElement().getY() * 2, playerPtr->getPlayerPos()->getHeadElement().getX() * 3);
    // cout << SNAKE_HEAD_CHAR;
}
void clear_dynamic_elements(void)
{
    // // Clear Previous Score
    // TERMINAL_CURSOR_JUMP(1, 0);
    // cout << setw(4) << setfill('0') << gameMech->getScore() << " / " << MAX_SCORE << " ";

    // Clear Previous Snake Positions
    // Clear Previous Snake Head & Snake Tail if size is greater than 1
    if (OBJ_SIZE > 1)
    {
        TERMINAL_CURSOR_JUMP(prevTailPos->getY() * 3, prevTailPos->getX() * 2);
        cout << EMPTY_CHAR;
        TERMINAL_CURSOR_JUMP(prevHeadPos->getY() * 3, prevHeadPos->getX() * 2);
        cout << playerPtr->getPlayerPos()->getElement(2).getSymbol();
    }
    // Clear Previous Snake Head Position if size is 1
    else if (OBJ_SIZE == 1)
    {
        TERMINAL_CURSOR_JUMP(prevHeadPos->getY() * 3, prevHeadPos->getX() * 2);
        cout << playerPtr->getPlayerPos()->getHeadElement().getSymbol();
    }
    else
        std::throw_with_nested(std::runtime_error("Invalid Snake Size"));

    // Clear Previous Food Position
    if (prevFoodPos->getX() == 1)
    {
        TERMINAL_CURSOR_JUMP(prevFoodPos->getY() * 3, prevFoodPos->getX() * 2 - 1);
        cout << BORDER_CHAR_BLANK;
    }
    else
    {
        TERMINAL_CURSOR_JUMP(prevFoodPos->getY() * 3, prevFoodPos->getX() * 2 - 1);
        cout << SPACE_CHAR;
    }
}
void print_score(int n)
{
    TERMINAL_CURSOR_JUMP(1, 0);
    cout << setw(n) << setfill('0') << gameMech->getScore() << " / " << MAX_SCORE << " ";
}
void print_snake(void)
{
    // Print only head if size is 1
    if (OBJ_SIZE == 1)
    {
        TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getHeadElement().getY() * 3, playerPtr->getPlayerPos()->getHeadElement().getX() * 2);
        cout << playerPtr->getPlayerPos()->getHeadElement().getSymbol();
    }
    // Print Snake Head & Tail if size is greater than 1
    else if (OBJ_SIZE > 1)
    {
        TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getElement(2).getY() * 3, playerPtr->getPlayerPos()->getElement(2).getX() * 2);
        cout << playerPtr->getPlayerPos()->getElement(2).getSymbol();

        TERMINAL_CURSOR_JUMP(playerPtr->getPlayerPos()->getHeadElement().getY() * 3, playerPtr->getPlayerPos()->getHeadElement().getX() * 2);
        cout << playerPtr->getPlayerPos()->getHeadElement().getSymbol();
    }
}
void print_food(void)
{
    TERMINAL_CURSOR_JUMP(gameMech->getFoodElement().getY() * 3, gameMech->getFoodElement().getX() * 2 - 1);
    cout << static_cast<char>('A' + gameMech->getFoodElement().getX()) << setw(2) << setfill('0') << gameMech->getFoodElement().getY();

    TERMINAL_CURSOR_JUMP(gameMech->getFoodElement().getY() * 3, gameMech->getFoodElement().getX() * 2);
    cout << gameMech->getFoodElement().getSymbol();
}
void store_prev_pos(void)
{
    *prevHeadPos = playerPtr->getPlayerPos()->getHeadElement();
    if (playerPtr->getPlayerPos()->getSize() > 1)
        *prevTailPos = playerPtr->getPlayerPos()->getTailElement();
    *prevFoodPos = gameMech->getFoodElement();
}
// ================================================================================
// ================================================================================
// ================================================================================
void DrawScreen(void)
{
    printRunTime = chrono::high_resolution_clock::now();
    objPos prevTail = playerPtr->getPlayerPos()->getTailElement(); // Define prevTail

    if (firstRun /*|| playerPtr->getDir() == Direction::STOP*/)
    {
        first_print();
        firstRun = false;
        return;
    }

    clear_dynamic_elements();

    print_score(digits);

    print_snake();

    print_food();

    store_prev_pos();
    

    // Processing Time Calculation
    endTime = chrono::high_resolution_clock::now();
    del_t = endTime - startTime;
    logic_t = logicRunTime - startTime;
    print_t = endTime - printRunTime;

    TERMINAL_CURSOR_JUMP(0 , (HEIGHT + 1) * 2);
    cout << "Total Time: " << del_t.count() << "ms" << endl;
    cout << "Logic Time: " << logic_t.count() << "ms" << endl;
    cout << "Print Time: " << print_t.count() << "ms" << endl;

    cout << "Score: " << gameMech->getScore() << " / " << MAX_SCORE << " ";
    return;
}

void LoopDelay(void)
{
    usleep(static_cast<int>(DELAY_CONST)); // 0.1s delay
}

void CleanUp(void)
{
    TERMINAL_CLEAR();

    // Display Win/Lose Message
    if (gameMech->getLoseFlagStatus())
    {
        TERMINAL_COLOR(12, 0);
        TERMINAL_DELAY_SINGLE_LINE_printf("                                                                       .-')      ('-.            \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("                                                                      ( OO ).  _(  OO)           \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("    ,--.   ,--..-'),-----.  ,--. ,--.          ,--.      .-'),-----. (_)---\\_)(,------.         \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("     \\  `.'  /( OO'  .-.  ' |  | |  |          |  |.-') ( OO'  .-.  '/    _ |  |  .---'         \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("   .-')     / /   |  | |  | |  | | .-')        |  | OO )/   |  | |  |\\  :` `.  |  |             \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("  (OO  \\   /  \\_) |  |\\|  | |  |_|( OO )       |  |`-' |\\_) |  |\\|  | '..`''.)(|  '--.      \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("   |   /  /\\_   \\ |  | |  | |  | | `-' /      (|  '---.'  \\ |  | |  |.-._)   \\ |  .--'       \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("   `-./  /.__)   `'  '-'  '('  '-'(_.-'        |      |    `'  '-'  '\\       / |  `---.         \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("     `--'          `-----'   `-----'           `------'      `-----'  `-----'  `------'        \n\n");
    }
    else
    {
        TERMINAL_COLOR(10, 0);
        TERMINAL_DELAY_SINGLE_LINE_printf("\n     ____     __   ,-----.      ___    _         .--.      .--..-./`) ,---.   .--.          _ _  .--.              \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("     \\   \\   /  /.'  .-,  '.  .'   |  | |        |  |_     |  |\\ .-.')|    \\  |  |         ( ` ) `-. \\          \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("      \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        | _( )_   |  |/ `-' \\|  ,  \\ |  |        (_ o _)   \\_\\       \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("       _( )_ .';  \\  '_ /  | :.'  '_  | |        |(_ o _)  |  | `-'`\"`|  |\\_ \\|  |         (_,_)   _( )_         \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("   ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|        | (_,_) \\ |  | .---. |  _( )_\\  |                (_ o _)        \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("  |   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        |  |/    \\|  | |   | | (_ o _)  |           _     (_,_)          \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("  |   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)        |  '  /\\  `  | |   | |  (_,_)\\  |         _( )_    / /         \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("   \\      /      '. \\_/``\".'   \\ /  . \\ /        |    /  \\    | |   | |  |    |  |        (_ o _).-' /         \n");
        TERMINAL_DELAY_SINGLE_LINE_printf("    `-..-'         '-----'      ``-'`-''         `---'    `---` '---' '--'    '--'         (_,_) `--'              \n\n");
    }

    TERMINAL_COLOR(15, 0);

    // De-allocate Heap Memory
    delete playerPtr;

    MacUILib_uninit();
}
