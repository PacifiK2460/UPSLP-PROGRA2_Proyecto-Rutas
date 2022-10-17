#include "core.h"

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
            wprintf(L"%ls%lc%ls",gradient[i],L' ', NORMAL);
        }

        free(gradient);
    }
    
    { // Make input widgets for username and password input
        Widget UsernameInput = {TEXT_INPUT};
        {   // username input configurations
            // set x coords to the center of the screen
            int height, width;
            get_window_size(&height, &width);
            UsernameInput.widget.input.x = (width / 2) - 10;

            // set y coords to the center of the screen
            UsernameInput.widget.input.y = (height / 2) - 2;

            // set width and height
            UsernameInput.widget.input.width = 20;
            UsernameInput.widget.input.height = 1;

            //set text type
            UsernameInput.widget.input.type = input_type;

            // set text mode
            UsernameInput.widget.input.mode = PLAIN;

            // set title
            UsernameInput.widget.input.title = L"Username";
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

            //set text type
            PasswordInput.widget.input.type = ANY;

            // set text mode
            PasswordInput.widget.input.mode = PASSWORD;

            // set title
            PasswordInput.widget.input.title = L"Password";
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
        }

        listWidget widgets;
        llist_append(widgets.items, &UsernameInput);
        llist_append(widgets.items, &PasswordInput);
        llist_append(widgets.items, &AttemptLogin);

        focus(widgets);
    }
    
    getc(stdin);
}