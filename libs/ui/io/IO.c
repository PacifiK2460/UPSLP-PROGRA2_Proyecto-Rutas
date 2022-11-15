#include "../io.h"

#define MAX_TEXT_LENGTH 51

void cleanInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    };
}

int evaluarText(wchar_t *Dest, int lenght)
{
    if (!fgetws(Dest, lenght, stdin))
    {
        return -1;
    }
    Dest[lenght] = L'\0';
    Dest[wcsspn(Dest, L"\r\n")] = 0;

    return 1;
}

int evaluarInt(int *Dest)
{
    wchar_t buff[1024];
    if (!fgetws(buff, 1024, stdin))
    {
        return -1;
    }

    wchar_t *endptr;
    errno = 0;
    *Dest = wcstol(buff, &endptr, 10);

    if ((errno == ERANGE || endptr == buff) || (*endptr && *endptr != '\n'))
    {
        *Dest = 0;
    }

    return 1;
}

int evaluarDouble(double *Dest)
{
    wchar_t buff[1024];
    if (!fgetws(buff, 1024, stdin))
    {
        return -1;
    }

    wchar_t *endptr;
    errno = 0;
    *Dest = wcstol(buff, &endptr, 10);

    if ((errno == ERANGE || endptr == buff) || (*endptr && *endptr != '\n'))
    {
        *Dest = 0;
    }

    return 1;
}

int input(wchar_t *bg_titulo, wchar_t *titulo, void *dest, int (*funcion)(void *))
{
    delimitador result = funcion;
    int res;
    do
    {
        {
            clearerr(stdin);
            wprintf(CLEAR);
            echo();
            winprint(STDOUTPUT, 4, getrows(STDOUTPUT) - 2, RESET FRGB(185, 251, 192) L"ctrl + d" RESET DIM L" regresar ");
            winprint(STDOUTPUT, 4, 2, bg_titulo);
            winprint(STDOUTPUT, 5, (getrows(STDOUTPUT) / 2) - 1, titulo);
            winprint(STDOUTPUT, 4, (getrows(STDOUTPUT) / 2) - 1, MENUVLINE);
            winprint(STDOUTPUT, 5, (getrows(STDOUTPUT) / 2), L" ");
            winprint(STDOUTPUT, 4, (getrows(STDOUTPUT) / 2), MENUVLINE);
        }

    } while ((res = result(dest)) == 0);
    noEcho();
    clearerr(stdin);
    return res;
}