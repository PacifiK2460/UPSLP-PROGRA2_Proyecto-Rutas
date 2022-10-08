#include "../libs/proyecto/auth.h"
#include "../libs/proyecto/routes.h"
#include "../libs/proyecto/tui.h"
#include "../libs/proyecto/proyecto.h"

#include <stdio.h>
#include <wchar.h>
#include <locale.h>


int main() {

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
            printf("Error: %d\n", result.Error_state);
            return 1;
        }
    }

    {// Routes Management initialization
        Result result = loadAllRoutes();
        if (result.Error_state != OK) {
            printf("Error: %d\n", result.Error_state);
            return 1;
        }

    }
    
}