#include "logic.h"

void manageUsers(User user)
{
    if (user.type != ADMIN)
    {
        wprintf(L"Only admins can manage users\n");
        return;
    }

    wchar_t *options[] = {L"Create User", L"Delete User", L"Modify User", L"List Users", L"Back"};
    wchar_t *descriptions[] = {L"Create a new user", L"Delete an existing user", L"Modify an existing user", L"List all users", L"Go back to main menu"};

    MENU menu;
    setMenuData(&menu, NULL, 0, 0, 5, options, descriptions);
}

// do
// {

//     switch (option)
//     {
//     case '0':
//         addUser();
//         break;
//     case '1':
//         editUser();
//         break;
//     case '2':
//         deleteUser();
//         break;
//     case '3':
//         listAllUsers();
//         break;
//     case '4':
//         listAdminUsers();
//         break;
//     case '5':
//         listNormalUsers();
//         break;
//     case '6':
//         queryUser();
//         break;
//     case '7':
//         break;
//     default:
//         wprintf(L"Invalid option\n");
//         getwchar();
//         break;
//     }
// } while (1);
// }
void manageRoutes(User user);
void queryLog(User user);
void registerNextRoute(User user);
void checkIn(User user);
void checkOut(User user);
void DebugData(User user);

<<<<<<< HEAD
=======
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

>>>>>>> ff6fdf7ba94ff897ebd530da505ff1f7c5ef1ec6
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
    wchar_t *options[] = {L"Manage Users", L"Manage Routes", L"Query Log", L"Register Next Route", L"Check In", L"Check Out", L"Debug Data"};
    wchar_t *descriptions[] = {L"Manage users", L"Manage routes", L"Query log", L"Register next route", L"Check in", L"Check out", L"Debug data"};
    MENU mainscreen;
    setMenuData(&mainscreen, NULL, 0, 0, 7, options, descriptions);

    Funciones mainFuncs[] = {
        (void*)&manageUsers,
        (void*)&manageRoutes,
        (void*)&queryLog,
        (void*)&registerNextRoute,
        (void*)&checkIn,
        (void*)&checkOut,
        (void*)&DebugData};

    while (1)
    {
        focusMenu(&mainscreen);
        if (mainscreen.selected == 7)
            break;

        if (mainscreen.selected < 0 || mainscreen.selected > 7)
            continue;

<<<<<<< HEAD
        // execute the function by index
        mainFuncs[mainscreen.selected]((void*)user);
=======
        funciones[mainscreen.selected](user);
>>>>>>> ff6fdf7ba94ff897ebd530da505ff1f7c5ef1ec6
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