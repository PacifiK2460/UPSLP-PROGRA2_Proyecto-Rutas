#include "tui.h"

Result initTUI(){
    Result result;
    
    result.Error_state = OK;

    // if in windows, use the windows API to enable ANSI escape codes
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_ANSI;
            return result;
        }
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_ANSI;
            return result;
        }
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_ANSI;
            return result;
        }
    #endif

    // if in linux, nothing needs to be done

    setlocale(LC_CTYPE, "");

    NEW_SCREEN();

    return result;
}

void gotoxy(int x, int y){
    printf("\e[%d;%df", y, x);
}

void print_status_bar(){
    // todo: print the status bar
}

wchar_t* FRGB(int r, int g, int b){
    wchar_t color[20];
    swprintf(color, 20, L"\e[38;2;%d;%d;%dm", r, g, b);
    return color;
}

wchar_t* BRGB(int r, int g, int b){
    wchar_t color[20];
    swprintf(color, 20, L"\e[48;2;%d;%d;%dm", r, g, b);
    return color;
}

// Test the code to see if it works

Result noEcho(){
    Result result;
    result.Error_state = OK;

    //if windows
    #ifdef _WIN32
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE) {
            result.Error_state = WINDOWS_UNABLE_TO_DISABLE_ECHO;
            return result;
        }
        DWORD dwMode = 0;
        if (!GetConsoleMode(hIn, &dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_DISABLE_ECHO;
            return result;
        }
        dwMode &= ~ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hIn, dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_DISABLE_ECHO;
            return result;
        }
    #endif

    //if linux
    #ifdef __linux__
        struct termios tty;
        tcgetattr(0, &tty);
        tty.c_lflag &= ~ECHO;
        tcsetattr(0, TCSANOW, &tty);
    #endif

    return result;
}

Result echo(){
    Result result;
    result.Error_state = OK;

    //if windows
    #ifdef _WIN32
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_ECHO;
            return result;
        }
        DWORD dwMode = 0;
        if (!GetConsoleMode(hIn, &dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_ECHO;
            return result;
        }
        dwMode |= ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hIn, dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_ECHO;
            return result;
        }
    #endif

    //if linux
    #ifdef __linux__
        struct termios tty;
        tcgetattr(0, &tty);
        tty.c_lflag |= ECHO;
        tcsetattr(0, TCSANOW, &tty);
    #endif

    return result;
}

Result rawMode(){
    Result result;
    result.Error_state = OK;

    //if windows
    #ifdef _WIN32
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_RAW_MODE;
            return result;
        }
        DWORD dwMode = 0;
        if (!GetConsoleMode(hIn, &dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_RAW_MODE;
            return result;
        }
        dwMode |= ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hIn, dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_RAW_MODE;
            return result;
        }
    #endif

    //if linux
    #ifdef __linux__
        struct termios tty;
        tcgetattr(0, &tty);
        tty.c_lflag |= ECHO;
        tcsetattr(0, TCSANOW, &tty);
    #endif

    return result;
}

Result cookedMode(){
    Result result;
    result.Error_state = OK;

    //if windows
    #ifdef _WIN32
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn == INVALID_HANDLE_VALUE) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_COOKED_MODE;
            return result;
        }
        DWORD dwMode = 0;
        if (!GetConsoleMode(hIn, &dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_COOKED_MODE;
            return result;
        }
        dwMode |= ENABLE_ECHO_INPUT;
        if (!SetConsoleMode(hIn, dwMode)) {
            result.Error_state = WINDOWS_UNABLE_TO_ENABLE_COOKED_MODE;
            return result;
        }
    #endif

    //if linux
    #ifdef __linux__
        struct termios tty;
        tcgetattr(0, &tty);
        tty.c_lflag |= ECHO;
        tcsetattr(0, TCSANOW, &tty);
    #endif

    return result;
}

// Stop testing

void get_window_size(int *rows, int *cols){
    // if windows
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
    #endif

    // if linux
    #ifdef __linux__
        struct winsize ws;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        *rows = ws.ws_row;
        *cols = ws.ws_col;
    #endif
}


Result focus(LList *widgets){
    // TODO: Make this work
}