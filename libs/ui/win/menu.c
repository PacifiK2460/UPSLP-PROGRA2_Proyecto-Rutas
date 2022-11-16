#include "../win.h"

// Estructura que nos permite habilitar y deshabilitar el eco
#ifdef _WIN32
DWORD mode;
#endif

#ifdef __linux__
struct termios mode;
#endif

void noEcho()
{
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
#endif

#ifdef __linux__
    tcgetattr(0, &mode);
    mode.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &mode);
#endif
}

void echo()
{
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode | (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
#endif

#ifdef __linux__
    tcgetattr(0, &mode);
    mode.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &mode);
#endif
}

void setMenuData(MENU *Destination, WINDOW *Parent, int x, int y, int pages, int rows, wchar_t *opciones[], wchar_t *descripciones[], Funciones __before, void *__before_args)
{
    Destination->Parent = Parent;
    Destination->X = x;
    Destination->Y = y;
    Destination->PAGES = pages;
    Destination->ROWS = rows;
    Destination->opciones = opciones;
    Destination->descripcion = descripciones;
    Destination->PAGE = 0;
    Destination->selected = 0;
    Destination->__before = __before;
    Destination->__before_args = __before_args;
}

void focusMenu(MENU *menu)
{
    updateMenu(menu);
    wprintf(CLEAR);
    wprintf(HIDE_CURSOR);

    if(menu->__before != NULL)
        menu->__before(menu->__before_args);

    // fflush(stdout);
    // Leemos el teclado non-canonical mode

    while (1)
    {
        updateMenu(menu);
        // echo();
        noEcho();
        wint_t c = getwchar();

        if (c == L'\033')
        {
            getwchar();
            c = getwchar();
            if (c == L'A')
            {
                if (menu->selected > 0)
                {
                    menu->selected--;
                }
            }
            else if (c == L'B')
            {
                if (menu->selected < menu->ROWS - 1)
                {
                    menu->selected++;
                }
            }
            else if (c == L'C')
            {
                if (menu->PAGE < menu->PAGES - 1)
                {
                    menu->PAGE++;
                }
            }
            else if (c == L'D')
            {
                if (menu->PAGE > 0)
                {
                    menu->PAGE--;
                }
            }
            else
            {
                continue;
            }
        }
        else if (c == 10)
        {
            return;
        } else if(c == WEOF){
            menu->selected = -1;
            return;
        }
        else
        {
            continue;
        }

        updateMenu(menu);
        // assertm(c != L'\033', "Unable to read scape sequence");
    }
    // echo();
    // Regresamos index seleccionado
}

void updateMenu(MENU *menu)
{
    for (int i = 0, x = 0; i < menu->ROWS; i++)
    {
        wchar_t color[50] = RESET;
        wchar_t separator[50] = RESET MENUVLINE;

        if (menu->selected == i)
        {
            wcscpy(color, FRGB(185, 251, 192));
            wcscpy(separator, FRGB(185, 251, 192) L"▶");
        }

        wprintf(BOLD L"\e[%i;%iH"
                     "%ls"
                     "%ls "
                     "%ls" RESET,
                menu->X + i + x++, menu->Y, // Posicion
                color, separator,
                menu->opciones[i]);
        wprintf(DIM L"\e[%i;%iH"
                    "%ls"
                    "%ls "
                    "%ls" RESET,
                menu->X + i + x++, menu->Y,
                color, separator,
                menu->descripcion[i]); // Cantidad de caracteres, texto
    }
}