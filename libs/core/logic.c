#include "logic.h"

void manageUsers(User user)
{
    if (user.type != ADMIN)
    {
        wprintf(L"Only admins can manage users\n");
        return;
    }

    wint_t option = 0;

    do
    {
        wprintf(CLEAR_SCREEN);
        wprintf(
            "0)"
            " " BOLD "Add user" NORMAL "\n"
            "1)"
            " " BOLD "Edit user" NORMAL "\n"
            "2)"
            " " BOLD "Delete user" NORMAL "\n"
            "3)"
            " " BOLD "List all users" NORMAL "\n"
            "4)"
            " " BOLD "List admin users" NORMAL "\n"
            "5)"
            " " BOLD "List normal users" NORMAL "\n"
            "6)"
            " " BOLD "Query user by UserName" NORMAL "\n"
            "7)"
            " " BOLD "Return to main menu" NORMAL "\n");

        wprintf(L"\nSelect an option: \n" RED L"%ls🔥$> " NORMAL, user.name);
        option = getwchar();

        switch (option)
        {
        case '0':
            addUser();
            break;
        case '1':
            editUser();
            break;
        case '2':
            deleteUser();
            break;
        case '3':
            listAllUsers();
            break;
        case '4':
            listAdminUsers();
            break;
        case '5':
            listNormalUsers();
            break;
        case '6':
            queryUser();
            break;
        case '7':
            break;
        default:
            wprintf(L"Invalid option\n");
            getwchar();
            break;
        }
    } while (1);
}
void manageRoutes(User user);
void queryLog(User user);
void registerNextRoute(User user);
void checkIn(User user);
void checkOut(User user);
void DebugData(User user);

typedef struct
{
    int length;
    wchar_t *text;
} Handshake;

int makeHandShake(void *data)
{
    Handshake *handshake = (Handshake *)data;
    handshake->text = calloc(handshake->length, sizeof(wchar_t));
    return evaluarText(handshake->text, handshake->length);
}

void *funcionesl = {
    manageUsers,
    manageRoutes,
    queryLog,
    registerNextRoute,
    checkIn,
    checkOut,
    DebugData};

void mainScreen(User *user)
{
    MENU mainscreen = {
        .title = L"Main Menu",
        .options = {
            L"Manage Users",
            L"Manage Routes",
            L"Query Log",
            L"Register Next Route",
            L"Check In",
            L"Check Out",
            L"Debug Data",
            L"Exit"},
        .length = 8,
        .funciones = funcionesl};
    }

    wint_t opcion;
    MENU mainscreen = {
        .Parent = NULL,
        .X = 0,
        .Y = 0,
        .ROWS = 8,
        .opciones = {
            "Manage users",
            "Manage routes",
            "Query log",
            "Register next route",
            "Check in",
            "Check out",
            "Debug data",
            "Exit",
        },
        .descripcion = {
            "Add, edit, delete, list and query users",
            "Add, edit, delete, list and query routes",
            "Query log",
            "Register next route",
            "Check in",
            "Check out",
            "Debug data",
            "Exit",
        },
    };

    while (1)
    {
        focusMenu(&mainscreen);
        if (mainscreen.selected == 7)
            break;

        if (mainscreen.selected < 0 || mainscreen.selected > 7)
            continue;

        funciones[mainscreen.selected](user);
    }
}

int TuiLogin()
{
    while (1)
    {
        wprintf(CLEAR);
        Handshake username = {
            .length = USERNAME_MAX_LENGTH + 1,
            .text = NULL};

        Handshake password = {
            .length = PASSWORD_MAX_LENGTH + 1,
            .text = NULL};

        if (input(L"Ingresa tu usuario", L"Nombre de Usuario", (void *)&username, makeHandShake) == 0)
            return 0;

        if (input(L"Ingresa tu contraseña", L"Contraseña", (void *)&password, makeHandShake) == 0)
            return 0;

        Result loginAttempt = login(username.text, password.text);

        switch (loginAttempt.Error_state)
        {
        case OK:
        {
            free(username.text);
            free(password.text);
            mainScreen(loginAttempt.Result);
            return 1;
        }
        case USER_NOT_FOUND:
        {
            free(username.text);
            free(password.text);
            wprintf(BOLD L"User not found! 😢\n" RESET "Press any key to continue...");
            getwc(stdin);
            break;
        }
        case INCORRECT_PASSWORD:
        {
            free(username.text);
            free(password.text);
            wprintf(BOLD L"Incorrect password! 😢\n" RESET "Press any key to continue...");
            getwc(stdin);
            break;
        }
        case USER_DISABLED:
        {
            free(username.text);
            free(password.text);
            wprintf(BOLD L"User disabled! 😢\n" RESET "Press any key to continue...");
            getwc(stdin);
            break;
        }
        }
    }
    return 1;
}