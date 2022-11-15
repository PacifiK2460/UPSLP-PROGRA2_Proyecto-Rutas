#include "libs/core/core.h"
#include "libs/auth/auth.h"
#include "libs/routes/routes.h"
// #include "libs/tui/tui.h"
#include "libs/core/logic.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>

void cleanUp()
{
    freeUsers();
    freeRoutes();
    CLOSE_SCREEN();

    wprintf(BOLD L"Goodbye! 🗃\n" RESET);
}

int main() {

    atexit(cleanUp);

    {// TUI initialization
        Result result = initTUI();
        if (result.Error_state != OK) {
            printf("Error: %d\n", result.Error_state);
            return 1;
        }
    }

    {// User Management initialization
        Result result = loadAllUsers();
        if (result.Error_state != OK) {
            CLOSE_SCREEN();
            printf("Error: %d\n", result.Error_state);
            return 1;
        }
    }

    {// Routes Management initialization
        Result result = loadAllRoutes();
        if (result.Error_state != OK) {
            CLOSE_SCREEN();
            printf("Error: %d\n", result.Error_state);
            return 1;
        }

    }
    
    return TuiLogin();
}