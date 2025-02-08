#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <string>
#include <time.h>

// #pragma comment(lib, "Winmm.lib")

// TERMINAL I/O
int TERMINAL_CHECK_INPUT(void);

char TERMINAL_GET_INPUT(void);

// TERMINAL AUDIO
void TERMINAL_SOUND(int freq, int dur);

// TERMINAL APPEARANCE
void TERMINAL_SIZE(int height, int width);

void TERMINAL_COLOR(int txt_color, int bg_color);

// TERMINAL CURSOR POSITION
void TERMINAL_DELAY_SINGLE_LINE_printf(std::string str, int time = 1);

void TERMINAL_CURSOR_JUMP(int x, int y);

void TERMINAL_CURSOR_HIDE();

// TERMINAL CLOSE
void TERMINAL_CLEAR();

void TERMINAL_EXIT();

#endif