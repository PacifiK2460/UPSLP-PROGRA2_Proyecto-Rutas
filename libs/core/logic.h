#pragma once

#include "core.h"
// #include "../tui/tui.h"
// #include "../tui/input.h"

#include "../auth/auth.h"
#include <string.h>

#include "../UI/win.h"


/**
 * @brief First screen of program, asks for user and password
 * 
 * @return int Exit code
 */
int TuiLogin();
