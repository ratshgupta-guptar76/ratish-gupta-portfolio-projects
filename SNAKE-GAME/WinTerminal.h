#ifndef WINTERMINAL_H
#define WINTERMINAL_H

#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <time.h>

#pragma comment(lib, "Winmm.lib")

void TERMINAL_SOUND();

void TERMINAL_COLOR(int txt_color, int bg_color);

void TERMINAL_DELAY_SINGLE_LINE_printf(std::string str, int time = 0);

void TERMINAL_CURSOR_JUMP(int x, int y);

void TERMINAL_CURSOR_HIDE();

void TERMINAL_CLEAR();

void TERMINAL_EXIT();

#endif // WINTERMINAL_H