#include <windows.h>
#include "WinTerminal.h"

void TERMINAL_COLOR(int txt_color, int bg_color)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, (bg_color << 4) | txt_color);
}

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

void TERMINAL_CLEAR()
{
    system("cls");
}

void TERMINAL_EXIT()
{
    TERMINAL_CLEAR();
    TERMINAL_CURSOR_JUMP(0, 0);
    exit(0);
}