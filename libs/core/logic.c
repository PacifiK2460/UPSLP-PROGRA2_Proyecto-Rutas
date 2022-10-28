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

        switch(option){
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

void mainScreen(User *user)
{
    wint_t opcion;
    do
    {
        wprintf(CLEAR_SCREEN);
        { // print menu
            wprintf(
                "0)"
                " " BOLD L"Manage Users" NORMAL "\n"
                "1)"
                " " BOLD L"Manage Routes" NORMAL "\n"
                "2)"
                " " BOLD L"Query Log" NORMAL "\n"
                "3)"
                " " BOLD L"Register Next Route" NORMAL "\n"
                "4)"
                " " BOLD L"Check In @ Route" NORMAL "\n"
                "5)"
                " " BOLD L"Check out @ Route" NORMAL "\n"
                "6)"
                " " BOLD L"Debug Data" NORMAL "\n"
                "7)"
                " " BOLD L"Return to Log-In" NORMAL "\n");
        }

        wprintf(L"\nSelect an option: \n");

        if (user->type == ADMIN)
        {
            wprintf(RED L"%ls🔥$> " NORMAL, user->name);
        }
        else
        {
            wprintf(BLUE L"%ls%ls💠 $> " NORMAL, user->name);
        }

        opcion = getwchar();

        switch (opcion)
        {
        case '0':
            manageUsers(*user);
            break;
        case '1':
            manageRoutes(*user);
            break;
        case '2':
            queryLog(*user);
            break;
        case '3':
            registerNextRoute(*user);
            break;
        case '4':
            checkIn(*user);
            break;
        case '5':
            checkOut(*user);
            break;
        case '6':
            DebugData(*user);
            break;
        case '7':
            return;
        default:
            wprintf(L"Invalid option\n");
            getwchar();
            break;
        }

    } while (1);
}

int TuiLogin()
{
    while (1)
    {
        wprintf(CLEAR_SCREEN);
        wchar_t username[USERNAME_MAX_LENGTH + 1] = {0};
        wchar_t password[PASSWORD_MAX_LENGTH + 1] = {0};

        wprintf(BOLD "LOGIN 🔐\n" NORMAL);

        wprintf(BOLD "$> Username: " NORMAL);
        fgetws(username, USERNAME_MAX_LENGTH + 1, stdin);
        username[wcscspn(username, L"\r\n")] = 0;

        wprintf(BOLD "$> Password: " NORMAL);
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
            wprintf(BOLD L"User not found! 😢\n" NORMAL "Press any key to continue...");
            getwc(stdin);
            break;
        }
        case INCORRECT_PASSWORD:
        {
            wprintf(BOLD L"Incorrect password! 😢\n" NORMAL "Press any key to continue...");
            getwc(stdin);
            break;
        }
        case USER_DISABLED:
        {
            wprintf(BOLD L"User disabled! 😢\n" NORMAL "Press any key to continue...");
            getwc(stdin);
            break;
        }
        }
    }
    return 1;
}