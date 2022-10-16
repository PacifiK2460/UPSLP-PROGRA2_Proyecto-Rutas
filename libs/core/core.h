#ifndef CORE_H
#define CORE_H 1

#include "../tui/tui.h"

#include <wchar.h>
#include <string.h>

enum ErrorType{
    UserERR = 0x100,
    RouteERR = 0x200,
    SystemERR = 0x300,
};

enum SystemErrors
{
    OK = 0,
    MALLOC_FAULT = SystemERR,
    FILE_NOT_FOUND,
    FILE_PERMISSION_DENIED,
    FILE_OPEN_ERROR,
    FOPEN_FAULT,
    FILE_READ_ERROR,

    WINDOWS_UNABLE_TO_ENABLE_ANSI,
    WINDOWS_UNABLE_TO_DISABLE_ECHO,
    WINDOWS_UNABLE_TO_ENABLE_RAW_MODE,
    WINDOWS_UNABLE_TO_ENABLE_COOKED_MODE,

    INPUT_INTEGER_OVERFLOW,
    INPUT_FLOAT_OVERFLOW,
    INPUT_INVALID_INPUT,
    INPUT_INVALID_TYPE,
    INPUT_BUFFER_OVERFLOW,
    INPUT_BUFFER_UNDERFLOW,
    INPUT_BUFFER_NULL,
    INPUT_BUFFER_SIZE_ZERO,
    INPUT_BUFFER_SIZE_NEGATIVE,
    INPUT_BUFFER_SIZE_TOO_SMALL,
    INPUT_BUFFER_SIZE_TOO_LARGE,
    UNKOWN_INPUT_ERROR,
};

struct Result{
    // If an error occurs, this will be set in the error field
    int Error_state;
    // if not, the pointer may be safe to use
    void* Result;
} Result;

/**
 * @brief First screen of program, asks for user and password
 * 
 * @return int Exit code
 */
int TuiLogin();

#endif