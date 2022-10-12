#include "core.h"

int TuiLogin()
{
    { // Print temporary status bar
        // print gradient bar
        int height, width;
        get_window_size(&height, &width);

        // go to the bottom of the screen
        gotoxy(0, height - 1);

        // print the gradient bar
        COLOR start = {BACKGROUND, HEX, 0};
        strcpy(start.color.HEX.HEX, "FFD78A");
        COLOR end = {BACKGROUND, HEX, 0};
        strcpy(end.color.HEX.HEX, "F4762D");
        wchar_t **gradient = monogradient(start, end, width);

        for (int i = 0; i < width; i++)
        {
            wprintf(L"%s▉%s", gradient[i], NORMAL);
        }

        free(gradient);
    }
    getc(stdin);
}