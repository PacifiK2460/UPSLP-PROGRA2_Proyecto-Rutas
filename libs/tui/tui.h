#pragma once

// Needed to store the list of focusable widgets
#include "../llist/llist.h"
#include "../core/core.h"
#include "keyboard.h"

// #include "input.h"
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

/**
 * @brief The TUI class
 * This class is the driver for the TUI in this proyect.
 */

Result initTUI();

/**
 * @brief Positions the cursor at the given coordinates.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void gotoxy(int x, int y);

/**
 * @brief Creates a new window.
 *
 */
#define NEW_SCREEN() wprintf(L"\e[?1049h")

/**
 * @brief Deletes the current window.
 *
 */
#define CLOSE_SCREEN() wprintf(L"\e[?1049l")

/**
 * @brief Prints the status bar at the bottom of the screen.
 *
 */
void print_status_bar();

// Text styles
#define NORMAL L"\e[0m"
#define BOLD L"\e[1m"
#define DIM L"\e[2m"
#define ITALIC L"\e[3m"
#define INVERSE L"\e[7m"
#define HIDDEN L"\e[8m"
#define STRIKETHROUGH L"\e[9m"
#define UNDERLINE L"\e[4m"

// MISC
#define HIDE_CURSOR L"\e[?25l"
#define SHOW_CURSOR L"\e[?25h"

#define CLEAR_SCREEN L"\e[2J"

#define MOVE_CURSOR_UP L"\e[1A"
#define MOVE_CURSOR_DOWN L"\e[1B"
#define MOVE_CURSOR_RIGHT L"\e[1C"
#define MOVE_CURSOR_LEFT L"\e[1D"

// Cursor echo
Result noEcho();
Result echo();

// RAW Modes
Result rawMode();
Result cookedMode();

// Get window size
void get_window_size(int *rows, int *cols);

/**
 * @brief Input widget definition
 *
 */
typedef struct inputWidget
{
    // Position of the widget
    int x, y;
    // Size of the widget
    int width, height;

    // Type of input to accept
    int type;

    // Password mode
    enum text_mode
    {
        PLAIN,
        PASSWORD,
    } mode;

    // Tile of the widget
    wchar_t *title;

    wchar_t *text;
} inputWidget;

/**
 * @brief Focusable list widget definition
 *
 */
typedef struct listWidget
{

    // List of items
    LList items;

    // Index of the focused item
    int selected;
} listWidget;

/**
 * @brief Focusable button widget definition
 *
 */
typedef struct buttonWidget
{
    // Position of the widget
    int x, y;
    // Size of the widget
    int width, height;

    // Title of the widget
    wchar_t *title;

} buttonWidget;

/**
 * @brief Focus the first widget in the list.
 *
 *  Handles all input through the configured functions.
 *  Any other non-input is handled internally such as <enter>'s, <tab>'s, <arrow-key>'s etc.
 *
 * @param widgets List of widgets to focus
 *
 */
Result focus(listWidget list, void* (*RePrintScreen)(void* data), void* data);

/**
 * @brief Generic Widget type definition, meant to be used in the list of widgets.
 *
 */
typedef struct Widget
{
    // Type of widget
    enum widget_type
    {
        TEXT_INPUT,
        WIDGET_LIST,
        BUTTON,
    } type;

    // Pointer to the widget
    union widget
    {
        inputWidget input;
        listWidget list;
        buttonWidget button;
    } widget;

    enum widget_state
    {
        ACCEPTED,
        CANCELED,
        CHANGED,
        FOCUSED,
        UNFOCUSED,
    } state;

    // Function to call when the input is accepted
    Result (*on_accept)(struct Widget *widget);
    Arguments on_accept_arguments;

    // Function to call when the input is cancelled
    Result (*on_cancel)(struct Widget *widget);
    Arguments on_cancel_arguments;

    // Function to call when the input is changed
    Result (*on_change)(struct Widget *widget);
    Arguments on_change_arguments;

    // Focus / Unfocus handlers
    Result (*on_focus)(struct Widget *widget);
    Arguments on_focus_arguments;

    Result (*on_unfocus)(struct Widget *widget);
    Arguments on_unfocus_arguments;
} Widget;

// Color Definitions

/**
 * @brief Color Struct management
 *
 */
typedef struct COLOR
{
    enum COLOR_TYPE
    {
        FOREGROUND,
        BACKGROUND
    } Color_Type;

    enum COLOR_MODE
    {
        RGB,
        HEX,
        HSL,
    } Color_Mode;

    union color
    {
        struct RGB
        {
            int R;
            int G;
            int B;
        } RGB;

        struct HEX
        {
            char HEX[7];
        } HEX;

        struct HSL
        {
            int H;
            int S;
            int L;
        } HSL;
    } color;

} COLOR;

/**
 * @brief Converts the defined color to a string that can be used in the terminal.
 *
 * @param color The color to convert
 * @return wchar_t* The resulting string
 */
wchar_t *ColorString(COLOR color);

/**
 * @brief Mono gradient conversion
 *
 * @param start The initial color to begin the gradient
 * @param end The final color to end the gradient
 * @param steps The number of steps between the initial and final color
 * @return wchar_t** The resulting gradient array
 */
wchar_t **monogradient(COLOR start, COLOR end, int steps);

void errorScren(wchar_t *message);