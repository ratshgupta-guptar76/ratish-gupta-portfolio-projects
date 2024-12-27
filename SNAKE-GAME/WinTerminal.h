#ifndef WINTERMINAL_H
#define WINTERMINAL_H

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

void TERMINAL_COLOR(int txt_color, int bg_color);

void TERMINAL_CURSOR_JUMP(int x, int y);

void TERMINAL_CURSOR_HIDE();

void TERMINAL_CLEAR();

void TERMINAL_EXIT();

#endif // WINTERMINAL_H