#pragma once

#include "core.h"
// #include "../tui/tui.h"
// #include "../tui/input.h"

#include "../auth/auth.h"
#include <string.h>
#include <wchar.h>

<<<<<<< HEAD
#include "../ui/win.h"
=======
#include "../UI/win.h"
#include "../UI/io.h"
>>>>>>> ff6fdf7ba94ff897ebd530da505ff1f7c5ef1ec6


/**
 * @brief First screen of program, asks for user and password
 * 
 * @return int Exit code
 */
int TuiLogin();
