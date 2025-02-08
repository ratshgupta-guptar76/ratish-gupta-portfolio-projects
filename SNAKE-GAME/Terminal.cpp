#include "Terminal.h"

#ifdef WINDOWS
#include <iostream>
#include <windows.h>
#include <conio.h>

// ####################    TERMINAL I/O    #########################
int TERMINAL_CHECK_INPUT(void)
{
    return _kbhit();
}

char TERMINAL_INPUT(void)
{
    return _getch();
}
// ####################   TERMINAL SOUND   #########################
void TERMINAL_SOUND(int freq, int dur)
{
    Beep(freq, dur);
}

// ####################   TERMINAL APPEARANCE   ####################
void TERMINAL_SIZE(int height, int width)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {static_cast<SHORT>(width), static_cast<SHORT>(height)};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT rect = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

void TERMINAL_COLOR(int txt_color, int bg_color)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, (bg_color << 4) | txt_color);
}

void TERMINAL_DELAY_SINGLE_LINE_printf(std::string str, int time)
{
    for (char c : str)
    {
        printf("%c", c);
        Sleep(time);
    }
}

// #################   TERMINAL CURSOR POSITION   ##################
void TERMINAL_CURSOR_JUMP(int x, int y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursor = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
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

// ####################   TERMINAL CLOSE   #########################
void TERMINAL_CLEAR()
{
    system("cls");
}

void TERMINAL_EXIT()
{
    TERMINAL_CLEAR();
    exit(0);
}

#endif // WINDOWS

#ifdef POSIX
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

// ####################    TERMINAL I/O    #########################
int TERMINAL_CHECK_INPUT(void)
{
    char ch = getch();
		if(ch == ERR)
			return 0;
		else
		{
			ungetch(ch);
			return 1;
		}	
}

char TERMINAL_GET_INPUT(void)
{
    return getch();
}
// ####################   TERMINAL SOUND   #########################
void TERMINAL_SOUND(int freq, int dur)
{
    std::cout << "\a"; // Bell character as a placeholder
    usleep(dur * 1000);
}

// ####################   TERMINAL APPEARANCE   ####################
void TERMINAL_SIZE(int height, int width)
{
    printf("\e[8;%d;%dt", height, width);
}

void TERMINAL_COLOR(int txt_color, int bg_color)
{
    printf("\033[%d;%dm", txt_color + 30, bg_color + 40);
}

void TERMINAL_DELAY_SINGLE_LINE_printf(std::string str, int time)
{
    for (char c : str)
    {
        putchar(c);
        fflush(stdout);
        usleep(time * 1000);
    }
}

// #################   TERMINAL CURSOR POSITION   ##################
void TERMINAL_CURSOR_JUMP(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

void TERMINAL_CURSOR_HIDE()
{
    printf("\e[?25l");
}

// ####################   TERMINAL CLOSE   #########################
void TERMINAL_CLEAR()
{
    printf("\033[2J\033[H");
}

void TERMINAL_EXIT()
{
    TERMINAL_CLEAR();
    exit(0);
}

#endif // POSIX
