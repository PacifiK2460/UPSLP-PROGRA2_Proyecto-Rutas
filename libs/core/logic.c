#include "logic.h"

Widget UsernameInput;
Widget PasswordInput;

Result mainScreen(User *user)
{
    CLOSE_SCREEN();
    NEW_SCREEN();
    printf(CLEAR_SCREEN);
    {
        COLOR start = {BACKGROUND, RGB, {0, 170, 27}};
        COLOR end = {BACKGROUND, RGB, {71, 246, 144}};

        print_status_bar(start, end);
    }

    if (user->type == ADMIN)
    {
        AdminPanel(user);
    }
    else
    {
        UserPanel(user);
    }
}

Result startProgram()
{
    Result login_code = login(UsernameInput.widget.input.text, PasswordInput.widget.input.text);
    wchar_t *msg = NULL;
    switch (login_code.Error_state)
    {
    case USER_NOT_FOUND:
        msg = L"USER " BOLD "NOT FOUND" NORMAL;
        break;
    case USER_DISABLED:
        msg = L"USER" BOLD "DISABLED" NORMAL;
        break;
    case INCORRECT_PASSWORD:
        msg = L"INCORRECT " BOLD "PASSWORD" NORMAL;
        break;
    case OK:
        mainScreen(login_code.Result);
        return login_code;
    default:
        msg = NULL;
        break;
    }
    errorScren(msg);
}

Result prepareOutput(Widget *widget)
{
    int width = widget->widget.input.width;

    COLOR back = {BACKGROUND, RGB, {16, 16, 16}}, start, end;
    wchar_t *back_color, **gradient;

    back_color = ColorString(back);

    if (widget->state == FOCUSED || widget->state == ACCEPTED)
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
    if (widget->state != ACCEPTED)
        wprintf(L"%ls", DIM);
    else
        wprintf(L"%ls", BOLD);
    for (int i = 0; i < wcslen(widget->widget.input.title); i++)
    {
        wprintf(L"%ls%lc", gradient[i], widget->widget.input.title[i]);
    }
    wprintf(L"%ls", NORMAL);

    // print line for the input field
    gotoxy(widget->widget.input.x, widget->widget.input.y + 1);
    for (int i = 0; i < wcslen(widget->widget.input.text); i++)
    {
        wchar_t character;
        if (widget->widget.input.mode == PASSWORD)
            character = L'*';
        else
            character = widget->widget.input.text[i];
        wprintf(UNDERLINE L"%ls%ls%lc", back_color, gradient[i], character);
    }
    for (int i = wcslen(widget->widget.input.text); i < width; i++)
    {
        wprintf(UNDERLINE L"%ls%ls ", back_color, gradient[i]);
    }
    wprintf(L"%ls", NORMAL);

    Result result;
    if (widget->state == ACCEPTED)
    {

        int i;
        i = wcslen(widget->widget.input.text);
        gotoxy(widget->widget.input.x + i, widget->widget.input.y + 1);

        wprintf(SHOW_CURSOR);
        noEcho();
        rawMode();
        clearerr(stdin);

        wint_t c;

        while (1)
        {
            c = getwchar();
            switch (c)
            {
            default:
                if (i < widget->widget.input.width - 1)
                {
                    wprintf(UNDERLINE L"%ls%ls", back_color, gradient[i]);
                    if (widget->widget.input.mode == PASSWORD)
                        wprintf(L"*");
                    else
                        wprintf(L"%lc", c);
                    wprintf(L"%ls", NORMAL);
                    widget->widget.input.text[i++] = c;
                    widget->widget.input.text[i] = L'\0';
                }
                break;
            case KEY_DELETE:
                if (i > 0)
                {
                    widget->widget.input.text[--i] = L'\0';

                    wprintf(MOVE_CURSOR_LEFT);
                    wprintf(UNDERLINE L"%ls%ls ", back_color, gradient[i]);
                    wprintf(MOVE_CURSOR_LEFT);
                }
                break;
            case KEY_ESC:
                result.Error_state = INPUT_EXIT_REQUESTED;
                goto end;
                break;
            case KEY_SPACEBAR:
            case KEY_TAB:
                break;
            case KEY_ENTER:
                widget->widget.input.text[i] = L'\0';
                goto end;
                break;
            case WEOF:
                result.Error_state = INPUT_EOF;
                goto end;
                break;
            }
        }
    }
end:
    cookedMode();
    wprintf(HIDE_CURSOR);
    free(back_color);
    for (int i = 0; i < width; i++)
    {
        free(gradient[i]);
    }
}

Result buttonClick(Widget *widget)
{
    // Print the button in the following style
    // [Button Title] in the middle of the widget, starting at the given x and y coordinates
    // and ending in the width of the widget. If it is not selected, it will be printed in
    // the background color, otherwise it will be printed in the foreground color.

    Result result = {0};

    int width = widget->widget.button.width;
    int title_length = wcslen(widget->widget.button.title);

    COLOR back = {BACKGROUND, RGB, {16, 16, 16}}, start, end;
    wchar_t *back_color, **gradient;

    back_color = ColorString(back);
    if (back_color == NULL)
    {
        result.Error_state = MALLOC_FAULT;
        goto exit;
    }

    if (focused >= FOCUSED)
    {

        start.Color_Type = BACKGROUND;
        start.Color_Mode = RGB;
        start.color.RGB.R = 0;
        start.color.RGB.G = 58;
        start.color.RGB.B = 129;

        end.Color_Type = BACKGROUND;
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
    if (gradient == NULL)
    {
        result.Error_state = MALLOC_FAULT;
        goto exit;
    }

    gotoxy(widget->widget.button.x, widget->widget.button.y);
    if (focused < ACTIVE)
        wprintf(L"%ls", DIM);
    else
        wprintf(L"%ls", BOLD);

    // align the title in the middle of the widget
    int offset = (width - title_length) / 2;
    int i = 0;
    wchar_t *color;

    for (; i < offset; i++)
    {
        if (focused >= FOCUSED)
            color = gradient[i];
        else
            color = back_color;
        wprintf(L"%ls ", color);
    }
    for (; i < title_length + offset; i++)
    {
        if (focused >= FOCUSED)
            color = gradient[i];
        else
            color = back_color;
        wprintf(L"%ls%lc", color, widget->widget.button.title[i - offset]);
    }
    for (; i < width; i++)
    {
        if (focused >= FOCUSED)
            color = gradient[i];
        else
            color = back_color;
        wprintf(L"%ls ", color);
    }

    wprintf(L"%ls", NORMAL);

    if (focused == ACTIVE)
    {
        wint_t c;
        while (1)
        {
            c = getwchar();
            switch (c)
            {
            case KEY_ENTER:
                startProgram();
                goto exit;
                break;
            case KEY_ESC:
                result.Error_state = INPUT_EXIT_REQUESTED;
                goto exit;
                break;
            case WEOF:
                result.Error_state = INPUT_EOF;
                goto exit;
                break;
            }
        }
    }

exit:
    if (back_color != NULL)
        free(back_color);
    for (int i = 0; i < width; i++)
    {
        if (gradient[i] != NULL)
            free(gradient[i]);
    }
    if (gradient != NULL)
        free(gradient);
    return result;
}

void PrintLogin(void *data)
{
    wprintf(CLEAR_SCREEN);
    { // Print temporary status bar
        // print gradient bar

        // print the gradient bar
        COLOR start = {BACKGROUND, HEX, 0};
        strcpy(start.color.HEX.HEX, "003a81");
        COLOR end = {BACKGROUND, HEX, 0};
        strcpy(end.color.HEX.HEX, "00acff");

        print_status_bar(start, end);
    }

    { // Print help at the bottom of the screen
        int height, width;
        get_window_size(&height, &width);

        // print the help text
        gotoxy(0, height - 4);
        wprintf(BOLD L"[ESC]*2" NORMAL " to exit\n");
        wprintf(BOLD L"[↕]    " NORMAL " to select a widget\n");
        wprintf(BOLD L"[ENTER]" NORMAL " to focus on a widget\n");
    }
}

int TuiLogin()
{

    { // Make input widgets for username and password input
        listWidget widgets = {0};
        { // username input configurations
            // set x coords to the center of the screen
            int height, width;
            get_window_size(&height, &width);
            UsernameInput.widget.input.x = (width / 2) - 10;

            UsernameInput.type = TEXT_INPUT;

            // set y coords to the center of the screen
            UsernameInput.widget.input.y = (height / 2) - 4;

            // set width and height
            UsernameInput.widget.input.width = 20;
            UsernameInput.widget.input.height = 1;

            // set text type
            UsernameInput.widget.input.type = ALPHANUMERIC;

            // set text mode
            UsernameInput.widget.input.mode = PLAIN;

            // set title
            UsernameInput.widget.input.title = L"Username\0";
            UsernameInput.widget.input.text = (wchar_t *)calloc(UsernameInput.widget.input.width + 1, sizeof(wchar_t));

            UsernameInput.on_focus = prepareOutput;
            UsernameInput.on_unfocus = prepareOutput;
            UsernameInput.on_accept = prepareOutput;
            {
                Box input_arguments[] = {{.data_type = WIDGET, .data = &UsernameInput}};
                UsernameInput.on_focus_arguments.number_of_data = 1;
                UsernameInput.on_focus_arguments.arguments = input_arguments;
            }
        }

        {
            // password input configurations
            // set x coords to the center of the screen
            int height, width;
            get_window_size(&height, &width);
            PasswordInput.widget.input.x = (width / 2) - 10;

            PasswordInput.type = TEXT_INPUT;

            // set y coords to the center of the screen
            PasswordInput.widget.input.y = (height / 2) - 1;

            // set width and height
            PasswordInput.widget.input.width = 20;
            PasswordInput.widget.input.height = 1;

            // set text type
            PasswordInput.widget.input.type = ANY;

            // set text mode
            PasswordInput.widget.input.mode = PASSWORD;

            // set title
            PasswordInput.widget.input.title = L"Password\0";
            PasswordInput.widget.input.text = (wchar_t *)calloc(PasswordInput.widget.input.width + 1, sizeof(wchar_t));

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
            AttemptLogin.widget.button.title = L"LOGIN\0";

            AttemptLogin.on_focus = focusButton;
            AttemptLogin.on_unfocus = unfocusButton;
            AttemptLogin.on_accept = handleButtonClick;
        }

        llist_append(&widgets.items, &UsernameInput);
        llist_append(&widgets.items, &PasswordInput);
        llist_append(&widgets.items, &AttemptLogin);

        focus(widgets, PrintLogin, NULL);
    }

    return 1;
}