#include "tui.h"

struct Result initTUI(){
    struct Result result;
    
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
    wprintf(L"\e[%d;%df", y, x);
}

void print_status_bar(){
    // move to the bottom 
    int cols, rows;
    get_window_size(&rows, &cols);
    gotoxy(0, rows);
    wprintf(L"TEST");
}

// Test the code to see if it works

struct Result noEcho(){
    struct Result result = {OK, NULL};
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

struct Result echo(){
    struct Result result = {OK, NULL};
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

struct Result rawMode(){
    struct Result result = {OK, NULL};
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

struct Result cookedMode(){
    struct Result result = {OK, NULL};
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
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
        *rows = ws.ws_row;
        *cols = ws.ws_col;
    #endif
}


struct Result focus(LList *widgets){
    // TODO: Make this work
}

void ConvertColorToRGB(COLOR color)
{
    switch (color.Color_Mode)
    {
    case RGB:
        return; // already RGB
        break;
    case HEX:
    {
        int r = color.color.RGB.R % 256, g = color.color.RGB.G % 256, b = color.color.RGB.B % 256;

        sscanf(color.color.HEX.HEX, "%02x%02x%02x", &r, &g, &b);

        color.color.RGB.R = r;
        color.color.RGB.G = g;
        color.color.RGB.B = b;
    }
    break;
    case HSL:
    { // https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
        int r, g, b;

        int h = color.color.HSL.H % 360, s = color.color.HSL.S % 100, l = color.color.HSL.L % 100;

        float c = (1 - abs(2 * l - 1)) * s;
        float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
        float m = l - c / 2;

        if (h >= 0 && h < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (h >= 60 && h < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (h >= 120 && h < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (h >= 180 && h < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (h >= 240 && h < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else if (h >= 300 && h < 360)
        {
            r = c;
            g = 0;
            b = x;
        }

        color.color.RGB.R = (r + m) * 255;
        color.color.RGB.G = (g + m) * 255;
        color.color.RGB.B = (b + m) * 255;
    }
    break;
    }
    return;
}

wchar_t *ColorString(COLOR color)
{
    wchar_t *color_string = malloc(sizeof(wchar_t) * 20);
    if (color_string == NULL)
    {
        return NULL;
    }

    ConvertColorToRGB(color);

    switch (color.Color_Type)
    {
    case FOREGROUND:
        wcscat(color_string, L"\e[38;2;");
        break;
    default: // incluiding BACKGROUND
        wcscat(color_string, L"\e[48;2;");
        break;
    }

    wchar_t *r = malloc(sizeof(wchar_t) * 4);
    wchar_t *g = malloc(sizeof(wchar_t) * 4);
    wchar_t *b = malloc(sizeof(wchar_t) * 4);

    if (r == NULL || g == NULL || b == NULL)
    {
        return NULL;
    }

    swprintf(r, 4, L"%d;", color.color.RGB.R);
    swprintf(g, 4, L"%d;", color.color.RGB.G);
    swprintf(b, 4, L"%dm", color.color.RGB.B);

    wcscat(color_string, r);
    wcscat(color_string, g);
    wcscat(color_string, b);

    free(r);
    free(g);
    free(b);

    return color_string;
}

wchar_t** monogradient(COLOR start, COLOR end, int steps){
    wchar_t** gradient = malloc(sizeof(wchar_t*) * steps);
    if(gradient == NULL){
        return NULL;
    }

    ConvertColorToRGB(start);
    ConvertColorToRGB(end);

    int r = start.color.RGB.R, g = start.color.RGB.G, b = start.color.RGB.B;
    int r_step = (end.color.RGB.R - start.color.RGB.R) / steps;
    int g_step = (end.color.RGB.G - start.color.RGB.G) / steps;
    int b_step = (end.color.RGB.B - start.color.RGB.B) / steps;

    for(int i = 0; i < steps; i++){
        COLOR color = {BACKGROUND, RGB, {r, g, b}};
        gradient[i] = ColorString(color);
        r += r_step;
        g += g_step;
        b += b_step;
    }

    return gradient;
}