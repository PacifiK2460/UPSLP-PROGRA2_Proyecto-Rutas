#include "tui.h"

Result initTUI()
{
    Result result;

    result.Error_state = OK;

    setlocale(LC_ALL, "C.UTF-8");
    setlocale(LC_CTYPE, "C.UTF-8");

    NEW_SCREEN();
    wprintf(CLEAR_SCREEN);

    return result;
}

void gotoxy(int x, int y)
{
    wprintf(L"\e[%d;%df", y, x);
}

void print_status_bar(COLOR start, COLOR end)
{
    start.Color_Type = BACKGROUND;
    end.Color_Type = BACKGROUND;

    // make to function
    int height, width;
    get_window_size(&height, &width);

    // go to the bottom of the screen
    gotoxy(0, height);
    wchar_t **gradient = monogradient(start, end, width);

    for (int i = 0; i < width; i++)
    {
        wprintf(L"%ls%lc%ls", gradient[i], L' ', NORMAL);
    }

    free(gradient);
}

// Test the code to see if it works

Result noEcho()
{
    Result result = {OK, NULL};
    result.Error_state = OK;

    struct termios tty;
    tcgetattr(0, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &tty);

    return result;
}

Result echo()
{
    Result result = {OK, NULL};
    result.Error_state = OK;

    struct termios tty;
    tcgetattr(0, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &tty);

    return result;
}

Result rawMode()
{
    Result result = {OK, NULL};
    result.Error_state = OK;

    struct termios tty;
    tcgetattr(0, &tty);
    tty.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &tty);

    return result;
}

Result cookedMode()
{
    Result result = {OK, NULL};
    result.Error_state = OK;

    struct termios tty;
    tcgetattr(0, &tty);
    tty.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &tty);

    return result;
}

// Stop testing

void get_window_size(int *rows, int *cols)
{

// if linux
#ifdef __linux__
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    *rows = ws.ws_row;
    *cols = ws.ws_col;
#endif
}

Result focus(listWidget list, void* (*RePrintScreen)(void* data), void* data)
{
    // print the listed widgets
    Result result = {OK, NULL};

    Widget *selected;

    while (1)
    {
        // reprint the background TUI
        RePrintScreen(data);
        { // Printing the widgets
            for (int i = 0; i < llist_size(&list.items); i++)
            {
                // the printing is done by the focus/unfocus functions
                Widget *temp = llist_get(&list.items, i);
                if (i == list.selected)
                {
                    temp->state = FOCUSED;
                    if (temp->on_focus != NULL)
                    {
                        temp->on_focus(temp);
                    }
                    selected = temp;
                }
                else
                {
                    temp->state = UNFOCUSED;
                    if (temp->on_unfocus != NULL)
                    {
                        temp->on_unfocus(temp);
                    }
                }
            }
        }

        { // Input handling
            clearerr(stdin);
            noEcho();
            rawMode();
            wprintf(HIDE_CURSOR);
            { // input management
                wint_t c = getwc(stdin);
                switch (c)
                {
                case KEY_TAB:
                    if (list.selected < llist_size(&list.items) - 1)
                        list.selected++;
                    else
                        list.selected = 0;
                    break;
                case KEY_ENTER:
                    echo();
                    cookedMode();

                    selected->state = ACCEPTED;

                    if (selected->on_accept == NULL)
                    {
                        continue;
                    }

                    Result res = selected->on_accept(selected);

                    switch (res.Error_state)
                    {
                    case OK:
                        result.Error_state = OK;
                        break;
                    case INPUT_EXIT_REQUESTED:
                        if (selected->on_cancel == NULL)
                        {
                            continue;
                        }
                        selected->state = CANCELED;
                        selected->on_cancel(selected);
                        result.Error_state = INPUT_EXIT_REQUESTED;
                        break;
                    case INPUT_EOF:
                        if (selected->on_cancel == NULL)
                        {
                            continue;
                        }
                        selected->state = CANCELED;
                        selected->on_cancel(selected);
                        result.Error_state = INPUT_EOF;
                        break;
                    case INPUT_INVALID_INPUT:
                        if (selected->on_cancel == NULL)
                        {
                            continue;
                        }
                        selected->state = CANCELED;
                        selected->on_cancel(selected);
                        result.Error_state = INPUT_INVALID_INPUT;
                        break;
                    case UNKOWN_INPUT_ERROR:
                        if (selected->on_cancel == NULL)
                        {
                            continue;
                        }
                        selected->state = CANCELED;
                        selected->on_cancel(selected);
                        result.Error_state = UNKOWN_INPUT_ERROR;
                        break;
                    case INPUT_PREMATURE_EXIT:
                        if (selected->on_cancel == NULL)
                        {
                            continue;
                        }
                        selected->state = CHANGED;
                        selected->on_change(selected);
                        result.Error_state = INPUT_PREMATURE_EXIT;
                        break;
                    }

                    break;
                case EOF:
                    result.Error_state = INPUT_EOF;
                    goto exit;
                case KEY_ESC: // especial keys
                    switch (c = getwc(stdin))
                    {
                    case '[':
                        switch (c = getwc(stdin))
                        {
                        case 'A':
                            if (list.selected > 0)
                                list.selected--;
                            else
                                list.selected = llist_size(&list.items) - 1;
                            break;
                        case 'B':
                        case 'Z': // TAB; SHIFT+TAB
                            if (list.selected < llist_size(&list.items) - 1)
                                list.selected++;
                            else
                                list.selected = 0;
                            break;
                        }
                        break;
                    case KEY_ESC:
                        result.Error_state = INPUT_EXIT_REQUESTED;
                        goto exit;
                    // case KEY_UP:
                    //     if (list.selected > 0)
                    //         list.selected--;
                    //     else
                    //         list.selected = llist_size(&list.items) - 1;
                    //     break;
                    default:
                        break;
                        // case KEY_DOWN:
                        // case KEY_TAB:
                        //     if (list.selected < llist_size(&list.items) - 1)
                        //         list.selected++;
                        //     else
                        //         list.selected = 0;
                        //     break;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
exit:
    echo();
    cookedMode();
    wprintf(SHOW_CURSOR);
    return result;
}

void ConvertColorToRGB(COLOR *color)
{
    switch (color->Color_Mode)
    {
    case RGB:
        return; // already RGB
        break;
    case HEX:
    {
        int r, g, b; // = color->color.RGB.R % 256, g = color->color.RGB.G % 256, b = color->color.RGB.B % 256;

        sscanf(color->color.HEX.HEX, "%02x%02x%02x", &r, &g, &b);

        color->color.RGB.R = r;
        color->color.RGB.G = g;
        color->color.RGB.B = b;

        color->Color_Mode = RGB;
    }
    break;
    case HSL:
    { // https://stackoverflow.com/questions/2353211/hsl-to-rgb-color-conversion
        int r, g, b;

        int h = color->color.HSL.H % 360, s = color->color.HSL.S % 100, l = color->color.HSL.L % 100;

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

        color->color.RGB.R = (r + m) * 255;
        color->color.RGB.G = (g + m) * 255;
        color->color.RGB.B = (b + m) * 255;

        color->Color_Mode = RGB;
    }
    break;
    }
    return;
}

wchar_t *ColorString(COLOR color)
{
    wchar_t *color_string = calloc(30, sizeof(wchar_t));
    if (color_string == NULL)
    {
        return NULL;
    }

    ConvertColorToRGB(&color);

    switch (color.Color_Type)
    {
    case FOREGROUND:
        wcscpy(color_string, L"\e[38;2;");
        break;
    default: // incluiding BACKGROUND
        wcscpy(color_string, L"\e[48;2;");
        break;
    }

    wchar_t *r = calloc(10 + 1, sizeof(wchar_t));
    wchar_t *g = calloc(10 + 1, sizeof(wchar_t));
    wchar_t *b = calloc(10 + 1, sizeof(wchar_t));

    if (r == NULL || g == NULL || b == NULL)
    {
        free(color_string);
        return NULL;
    }

    swprintf(r, 6, L"%d;", color.color.RGB.R);
    swprintf(g, 6, L"%d;", color.color.RGB.G);
    swprintf(b, 6, L"%dm", color.color.RGB.B);

    wcscat(color_string, r);
    wcscat(color_string, g);
    wcscat(color_string, b);

    free(r);
    free(g);
    free(b);

    return color_string;
}

wchar_t **monogradient(COLOR start, COLOR end, int steps)
{
    wchar_t **gradient = malloc(sizeof(wchar_t *) * (steps + 1));
    if (gradient == NULL)
    {
        return NULL;
    }

    // Values are changed locally, fix
    ConvertColorToRGB(&start);
    ConvertColorToRGB(&end);

    // generate the evenly spaced gradient
    double r = start.color.RGB.R;
    double g = start.color.RGB.G;
    double b = start.color.RGB.B;

    // what if the steps are bigger than the difference between the colors?
    double r_step = ((double)end.color.RGB.R - (double)start.color.RGB.R) / (double)steps;
    double g_step = ((double)end.color.RGB.G - (double)start.color.RGB.G) / (double)steps;
    double b_step = ((double)end.color.RGB.B - (double)start.color.RGB.B) / (double)steps;

    for (int i = 0; i < steps; i++)
    {
        COLOR color = {start.Color_Type, RGB, {(int)r, (int)g, (int)b}};
        gradient[i] = ColorString(color);
        r += r_step;
        g += g_step;
        b += b_step;
    }

    gradient[steps] = NULL;

    return gradient;
}

void errorScren(wchar_t *message)
{
    wprintf(CLEAR_SCREEN);
    { // print message
        if (message == NULL)
        {
            message = L"Unknown error";
        }
        int word_length = wcslen(message);

        int height, width;
        get_window_size(&height, &width);

        int x = (width - word_length/2)/2;

        gotoxy(x, height / 2);
        wprintf(L"%ls", message);
    }

    { // print help
        int height, width;
        get_window_size(&height, &width);

        int x = (width - 20) / 2;

        gotoxy(x, height-1);
        wprintf(DIM L"Press any key to continue" NORMAL);
    }

    {// Error status bar
        COLOR start = {BACKGROUND, RGB, {236, 0, 0}};
        COLOR end = {BACKGROUND, RGB, {56, 0, 0}};
        print_status_bar(start, end);
    }

    getwchar();
}