#include "logic.h"

void prepareOutput(int focused, Widget *widget)
{
    int width = widget->widget.input.width;

    COLOR back = {BACKGROUND, RGB, {16,16,16}}, start, end;
    wchar_t *back_color, **gradient;

    back_color = ColorString(back);

    if (focused)
    {

        start.Color_Type = FOREGROUND;
        start.Color_Mode = RGB;
        start.color.RGB.R = 0;
        start.color.RGB.G = 58;
        start.color.RGB.B = 129;

        end.Color_Type = FOREGROUND;
        end.Color_Mode = RGB;
        end.color.RGB.R = 0;
        end.color.RGB.G = 172;
        end.color.RGB.B = 255;
    }
    else
    {
        start.Color_Type = FOREGROUND;
        start.Color_Mode = RGB;
        start.color.RGB.R = 50;
        start.color.RGB.G = 50;
        start.color.RGB.B = 50;

        end.Color_Type = FOREGROUND;
        end.Color_Mode = RGB;
        end.color.RGB.R = 100;
        end.color.RGB.G = 100;
        end.color.RGB.B = 100;
    }

    gradient = monogradient(start, end, width);

    gotoxy(widget->widget.input.x, widget->widget.input.y);
    wprintf(L"%ls", DIM);
    for (int i = 0; i < wcslen(widget->widget.input.title); i++)
    {
        wprintf(L"%ls%lc", gradient[i], widget->widget.input.title[i]);
    }
    wprintf(L"%ls", NORMAL);

    // print line for the input field
    gotoxy(widget->widget.input.x, widget->widget.input.y + 1);
    for (int i = 0; i < width; i++)
    {
        wprintf(UNDERLINE L"%ls%ls ", back_color, gradient[i]);
    }
    wprintf(L"%ls", NORMAL);

    free(back_color);
    for (int i = 0; i < width; i++)
    {
        free(gradient[i]);
    }
}

void focusText(Widget *widget)
{
    prepareOutput(1, widget);
}

void unfocusText(Widget *widget)
{
    prepareOutput(0, widget);
}

Result readInput(Widget *widget){
    Result result;
    result.Error_state = OK;
    
    wprintf(L"READED INPUT");

    return result;
}

void focusButton(Widget *widget)
{
    wprintf(L"UNIMPLEMENTED");
}

void unfocusButton(Widget *widget)
{
    wprintf(L"UNIMPLEMENTED");
}

int TuiLogin()
{
    { // Print temporary status bar
        // print gradient bar
        int height, width;
        get_window_size(&height, &width);

        // go to the bottom of the screen
        gotoxy(0, height);

        // print the gradient bar
        COLOR start = {BACKGROUND, HEX, 0};
        strcpy(start.color.HEX.HEX, "003a81");
        COLOR end = {BACKGROUND, HEX, 0};
        strcpy(end.color.HEX.HEX, "00acff");
        wchar_t **gradient = monogradient(start, end, width);

        for (int i = 0; i < width; i++)
        {
            wprintf(L"%ls%lc%ls", gradient[i], L' ', NORMAL);
        }

        free(gradient);
    }

    { // Make input widgets for username and password input
        Widget UsernameInput = {TEXT_INPUT};
        { // username input configurations
            // set x coords to the center of the screen
            int height, width;
            get_window_size(&height, &width);
            UsernameInput.widget.input.x = (width / 2) - 10;

            // set y coords to the center of the screen
            UsernameInput.widget.input.y = (height / 2) - 3;

            // set width and height
            UsernameInput.widget.input.width = 20;
            UsernameInput.widget.input.height = 1;

            // set text type
            UsernameInput.widget.input.type = ALPHANUMERIC;

            // set text mode
            UsernameInput.widget.input.mode = PLAIN;

            // set title
            UsernameInput.widget.input.title = L"Username\0";

            UsernameInput.on_focus = focusText;
            UsernameInput.on_unfocus = unfocusText;
            UsernameInput.on_accept = readInput;
        }
        Widget PasswordInput = {TEXT_INPUT};
        {
            // password input configurations
            // set x coords to the center of the screen
            int height, width;
            get_window_size(&height, &width);
            PasswordInput.widget.input.x = (width / 2) - 10;

            // set y coords to the center of the screen
            PasswordInput.widget.input.y = (height / 2);

            // set width and height
            PasswordInput.widget.input.width = 20;
            PasswordInput.widget.input.height = 1;

            // set text type
            PasswordInput.widget.input.type = ANY;

            // set text mode
            PasswordInput.widget.input.mode = PASSWORD;

            // set title
            PasswordInput.widget.input.title = L"Password\0";

            PasswordInput.on_focus = focusText;
            PasswordInput.on_unfocus = unfocusText;
            PasswordInput.on_accept = readInput;
        }

        Widget AttemptLogin = {BUTTON};
        {
            // set x coords to the center of the screen
            int height, width;
            get_window_size(&height, &width);
            AttemptLogin.widget.button.x = (width / 2) - 10;

            // set y coords to the center of the screen
            AttemptLogin.widget.button.y = (height / 2) + 2;

            // set width and height
            AttemptLogin.widget.button.width = 20;
            AttemptLogin.widget.button.height = 1;

            // set title
            AttemptLogin.widget.button.title = L"LOGIN";

            AttemptLogin.on_focus = focusButton;
            AttemptLogin.on_unfocus = unfocusButton;
        }

        listWidget widgets = {0};
        llist_append(&widgets.items, &UsernameInput);
        llist_append(&widgets.items, &PasswordInput);
        // llist_append(&widgets.items, &AttemptLogin);

        focus(widgets);
    }

    return 1;
}