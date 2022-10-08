#ifndef TUI_H
#define TUI_H

// Needed to store the list of focusable widgets
#include "../llist/llist.h"
#include "proyecto.h"
#include <wchar.h>
#include <locale.h>

// if in linux, use termios.h
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
    #include <conio.h>
    #include <windows.h>
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    // Below __linux__ check should be enough to handle Android,
    // but something may be unique to Android.
#elif __linux__
    // linux
    #include <termios.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif


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
#define NORMAL  L"\e[0m"
#define BOLD    L"\e[1m"
#define DIM     L"\e[2m"
#define ITALIC  L"\e[3m"
#define INVERSE L"\e[7m"

// Colors

/**
 * @brief Returns the colored-terminal-code for the given color in the foreground.
 *
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 *
 * @return wchar_t The color text-code.
 */
wchar_t *FRGB(int r, int g, int b);

/**
 * @brief Returns the colored-terminal-code for the given color in the background.
 *
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 *
 * @return wchar_t The color text-code.
 */
wchar_t *BRGB(int r, int g, int b);

// MISC
#define HIDE_CURSOR L"\e[?25l"
#define SHOW_CURSOR L"\e[?25h"

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
    Result (*on_accept)(wchar_t *input);

    // Function to call when the input is cancelled
    Result (*on_cancel)(void *opcional_data);

    // Function to call when the input is changed
    Result (*on_change)(wchar_t *input);

    // Focus / Unfocus handlers
    Result (*on_focus)(void *opcional_data);
    Result (*on_unfocus)(void *opcional_data);
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
    Result (*on_accept)(void *opcional_data);

    // Function to call when the input is cancelled
    Result (*on_cancel)(void *opcional_data);

    // Function to call when the focused item is changed
    Result (*on_change)(void *opcional_data);

    // Focus / Unfocus handlers
    Result (*on_focus)(void *opcional_data);
    Result (*on_unfocus)(void *opcional_data);
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
    Result (*on_press)(void *opcional_data);

    // Focus / Unfocus handlers
    Result (*on_focus)(void *opcional_data);
    Result (*on_unfocus)(void *opcional_data);
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
Result focus(LList *widgets);

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

#endif