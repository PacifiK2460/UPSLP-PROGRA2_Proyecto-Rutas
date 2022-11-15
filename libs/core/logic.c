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

void mainScreen(User *user)
{
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

        // execute the function by index
        mainFuncs[mainscreen.selected]((void*)user);
    }
}

int TuiLogin()
{
    while (1)
    {
        wprintf(CLEAR);
        wchar_t username[USERNAME_MAX_LENGTH + 1] = {0};
        wchar_t password[PASSWORD_MAX_LENGTH + 1] = {0};

        wprintf(BOLD "LOGIN 🔐\n" RESET);

        wprintf(BOLD "$> Username: " RESET);
        fgetws(username, USERNAME_MAX_LENGTH + 1, stdin);
        username[wcscspn(username, L"\r\n")] = 0;

        wprintf(BOLD "$> Password: " RESET);
        fgetws(password, PASSWORD_MAX_LENGTH + 1, stdin);
        password[wcscspn(password, L"\r\n")] = 0;

        Result loginAttempt = login(username, password);

        switch (loginAttempt.Error_state)
        {
        case OK:
        {
            mainScreen(loginAttempt.Result);
            return 1;
        }
        case USER_NOT_FOUND:
        {
            wprintf(BOLD L"User not found! 😢\n" RESET "Press any key to continue...");
            getwc(stdin);
            break;
        }
        case INCORRECT_PASSWORD:
        {
            wprintf(BOLD L"Incorrect password! 😢\n" RESET "Press any key to continue...");
            getwc(stdin);
            break;
        }
        case USER_DISABLED:
        {
            wprintf(BOLD L"User disabled! 😢\n" RESET "Press any key to continue...");
            getwc(stdin);
            break;
        }
        }
    }
    return 1;
}