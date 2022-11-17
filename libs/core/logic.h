#pragma once

#include "core.h"
// #include "../tui/tui.h"
// #include "../tui/input.h"

#include "../routes/routes.h"
#include "../auth/auth.h"
#include <string.h>
#include <wchar.h>

#include "../ui/win.h"
#include "../ui/io.h"


/**
 * @brief First screen of program, asks for user and password
 * 
 * @return int Exit code
 */
int TuiLogin();

typedef struct
{
    int length;
    wchar_t *text;
} Handshake;
