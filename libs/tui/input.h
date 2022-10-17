#pragma once

// extern struct Result Result;

#include "../core/core.h"
// #include "../tui/tui.h"

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>


enum input_type
{
    INTEGER,
    FLOAT,
    NUMERIC,
    ALPHA,
    ALPHANUMERIC,
    ANY,
};

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13

#define KEY_BACKSPACE 8
#define KEY_DELETE 127

#define KEY_ESC 27
#define KEY_TAB 9
#define KEY_SHIFT_n_TAB 353

struct Result input(int type, wchar_t *buffer, int buffer_size);
