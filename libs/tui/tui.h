#ifndef TUI_H
#define TUI_H 1

// Needed to store the list of focusable widgets
#include "../llist/llist.h"
#include "../core/core.h"

#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

extern struct Result Result;

/**
 * @brief The TUI class
 * This class is the driver for the TUI in this proyect.
 */

struct Result initTUI();

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
#define NORMAL  L"\e[0m"
#define BOLD    L"\e[1m"
#define DIM     L"\e[2m"
#define ITALIC  L"\e[3m"
#define INVERSE L"\e[7m"

// MISC
#define HIDE_CURSOR L"\e[?25l"
#define SHOW_CURSOR L"\e[?25h"

// Cursor echo
struct Result noEcho();
struct Result echo();

// RAW Modes
struct Result rawMode();
struct Result cookedMode();

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
    enum input_type
    {
        NUMERIC,
        ALPHA,
        ALPHANUMERIC,
        ANY,
    } type;

    // Password mode
    enum text_mode
    {
        PLAIN,
        PASSWORD,
    } mode;

    // Tile of the widget
    wchar_t *title;

    // Function to call when the input is accepted
    struct Result (*on_accept)(wchar_t *input);

    // Function to call when the input is cancelled
    struct Result (*on_cancel)(void *opcional_data);

    // Function to call when the input is changed
    struct Result (*on_change)(wchar_t *input);

    // Focus / Unfocus handlers
    struct Result (*on_focus)(void *opcional_data);
    struct Result (*on_unfocus)(void *opcional_data);
} inputWidget;

/**
 * @brief Focusable list widget definition
 *
 */
typedef struct listWidget
{
    // Position of the widget
    int x, y;
    // Size of the widget
    int width, height;

    // Title of the widget
    wchar_t *title;

    // List of items
    LList *items;

    // Index of the focused item
    int selected;

    // Function to call when the focused item is selected
    struct Result (*on_accept)(void *opcional_data);

    // Function to call when the input is cancelled
    struct Result (*on_cancel)(void *opcional_data);

    // Function to call when the focused item is changed
    struct Result (*on_change)(void *opcional_data);

    // Focus / Unfocus handlers
    struct Result (*on_focus)(void *opcional_data);
    struct Result (*on_unfocus)(void *opcional_data);
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

    // Function to call when the button is pressed
    struct Result (*on_press)(void *opcional_data);

    // Focus / Unfocus handlers
    struct Result (*on_focus)(void *opcional_data);
    struct Result (*on_unfocus)(void *opcional_data);
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
struct Result focus(LList *widgets);

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
        LIST,
        BUTTON,
    } type;

    // Pointer to the widget
    void *widget;
} Widget;

// Color Definitions

/**
 * @brief Color Struct management
 * 
 */
typedef struct COLOR{
    enum COLOR_TYPE{
        FOREGROUND,
        BACKGROUND
    } Color_Type;

    enum COLOR_MODE{
        RGB,
        HEX,
        HSL,        
    } Color_Mode;

    union color
    {
        struct RGB{
            char R;
            char G;
            char B;
        } RGB;

        struct HEX{
            char HEX[7];
        } HEX;

        struct HSL{
            char H;
            char S;
            char L;
        } HSL;
    } color;
    
} COLOR;

/**
 * @brief Converts the defined color to a string that can be used in the terminal. 
 * 
 * @param color The color to convert
 * @return wchar_t* The resulting string
 */
wchar_t* ColorString(COLOR color);

/**
 * @brief Mono gradient conversion
 * 
 * @param start The initial color to begin the gradient
 * @param end The final color to end the gradient
 * @param steps The number of steps between the initial and final color
 * @return wchar_t** The resulting gradient array
 */
wchar_t** monogradient(COLOR start, COLOR end, int steps);

#endif