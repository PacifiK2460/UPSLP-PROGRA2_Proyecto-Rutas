#include "libs/auth/auth.h"
#include "libs/routes/routes.h"
#include "libs/tui/tui.h"
#include "libs/core/core.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>

void cleanUp()
{
    freeUsers();
    freeRoutes();
    CLOSE_SCREEN();
}

int main() {

    atexit(cleanUp);

    {// TUI initialization
        struct Result result = initTUI();
        if (result.Error_state != OK) {
            printf("Error: %d\n", result.Error_state);
            return 1;
        }
    }

    {// User Management initialization
        struct Result result = loadAllUsers();
        if (result.Error_state != OK) {
            CLOSE_SCREEN();
            printf("Error: %d\n", result.Error_state);
            return 1;
        }
    }

    {// Routes Management initialization
        struct Result result = loadAllRoutes();
        if (result.Error_state != OK) {
            CLOSE_SCREEN();
            printf("Error: %d\n", result.Error_state);
            return 1;
        }

    }
    
    TuiLogin();
}