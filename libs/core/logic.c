#include "logic.h"

Widget UsernameInput;
Widget PasswordInput;

void mainScreen(User *user)
{
    printf(CLEAR_SCREEN);
    {// print menu
        wprintf(
           "1)" BOLD L"Manage Users" NORMAL "\n"
           "2)" BOLD L"Manage Routes" NORMAL "\n"
           "3)" BOLD L"Query Log" NORMAL "\n"
           "4)" BOLD L"Register Next Route" NORMAL "\n"
           "5)" BOLD L"Check In @ Route" NORMAL "\n"
           "6)" BOLD L"Check out @ Route" NORMAL "\n"
           "7)" BOLD L"Debug Data" NORMAL "\n"
           "8)" BOLD L"Return to Log-In" NORMAL "\n"
        );
    }

    wprintf(L"Select an option: \n" );

    if(user->type == ADMIN)
    {
        wprintf(RED L"%ls🔥 $>", user->name);
    }
    else
    {
        wprintf(BLUE L"%ls%ls💠 $>", user->name);
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
        getwc(username, USERNAME_MAX_LENGTH + 1);

        wprintf(BOLD "$> Password: " NORMAL);
        getcwd(password, PASSWORD_MAX_LENGTH + 1);

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