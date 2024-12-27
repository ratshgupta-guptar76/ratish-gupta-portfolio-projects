#include <windows.h>
#include "WinTerminal.h"

// #################################################################
// ####################   TERMINAL SOUND   #########################
// #################################################################

void TERMINAL_SOUND(int freq, int dur)
{
    Beep(freq, dur);
}

// #################################################################
// ####################   TERMINAL APPEARANCE   ####################
// #################################################################

void TERMINAL_COLOR(int txt_color, int bg_color)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, (bg_color << 4) | txt_color);
}

void TERMINAL_DELAY_SINGLE_LINE_printf(std::string str, int time)
{
    int n = str.length();

    if (time == 0)
        time = 1;

    for (int i = 0; i < n; ++i)
    {
        while (str[i] == ' ')
        {
            printf(" ");
            i++;
        }
        printf("%c", str[i]);
        Sleep(time);
    }    
}

// #################################################################
// #################   TERMINAL CURSOR POSITION   ##################
// #################################################################

void TERMINAL_CURSOR_JUMP(int x, int y)
{
    COORD cursor = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(console, cursor);
}

void TERMINAL_CURSOR_HIDE()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;

    GetConsoleCursorInfo(console, &cursor);
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(console, &cursor);
}

// #################################################################
// ####################   TERMINAL CLOSE   #########################
// #################################################################
void TERMINAL_CLEAR()
{
    COORD topLeft={0,0};
    HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console,&screen);
    FillConsoleOutputCharacterA(console,' ',screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
                               screen.dwSize.X * screen.dwSize.Y, topLeft,&written);
    SetConsoleCursorPosition(console,topLeft);
}

void TERMINAL_EXIT()
{
    TERMINAL_CLEAR();
    TERMINAL_CURSOR_JUMP(0, 0);
    exit(0);
}