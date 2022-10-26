#include "logic.h"

Widget UsernameInput;
Widget PasswordInput;

void mainScreen(User *user)
{
    wprintf(CLEAR_SCREEN);
    {// print menu
        wprintf(
           "1)" " " BOLD L"Manage Users" NORMAL "\n"
           "2)" " " BOLD L"Manage Routes" NORMAL "\n"
           "3)" " " BOLD L"Query Log" NORMAL "\n"
           "4)" " " BOLD L"Register Next Route" NORMAL "\n"
           "5)" " " BOLD L"Check In @ Route" NORMAL "\n"
           "6)" " " BOLD L"Check out @ Route" NORMAL "\n"
           "7)" " " BOLD L"Debug Data" NORMAL "\n"
           "8)" " " BOLD L"Return to Log-In" NORMAL "\n"
        );
    }

    wprintf(L"\nSelect an option: \n" );

    if(user->type == ADMIN)
    {
        wprintf(RED L"%ls🔥$> " NORMAL, user->name);
    }
    else
    {
        wprintf(BLUE L"%ls%ls💠 $> " NORMAL, user->name);
    }

    getwchar();
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