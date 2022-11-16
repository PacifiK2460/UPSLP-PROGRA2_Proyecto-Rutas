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
    setMenuData(&menu, NULL, 6, 4, 5, 0, options, descriptions, NULL, NULL);
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
void manageRoutes(User user) {}
void queryLog(User user) {}
void registerNextRoute(User user) {}
void checkIn(User user) {}
void checkOut(User user) {}
void DebugData(User user) {}

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

void mainScreenUI(void *data)
{
    printHelp(
        RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(185, 251, 192) L"↵" RESET DIM L" Escoger ",
        4, getrows(STDOUTPUT) - 2);

    User *user = (User *)data;
    wchar_t saludo[1024] = L"👋 Bienvenido " BOLD;
    wcscat(saludo, user->name);
    wcscat(saludo, RESET L", es un placer tenerte de vuelta.\0");

    winprint(STDOUTPUT, 4, 2, saludo);
    winprint(STDOUTPUT, 4, 3, DIM L"Escoge alguna actividad a realizar..." RESET "\0");
}

void mainScreen(User *user)
{

    wint_t opcion;
    wchar_t *options[] = {L"Administrar Usuarios", L"Administrar Rutas", L"Enlistar Usuarios", L"Reservar Asiento", L"Check In", L"Check Out", L"Debug Data", L"Cerrar Sesión", L"Cerrar Aplicación"};
    wchar_t *descriptions[] = {L"Agrega, elimina, u modifica los usuarios registrados", L"Agrega, elimina, u modifica las rutas registradas", L"Busca entre todos los usuarios registrados", L"Reserva el asiento de tu siguiente parada", L"Tomar registro de toma de ruta", L"Terminar ruta", L"Información detallada de la información registrada", L"Cierra sesión para que otro usuario pueda usar el sitema", L"Cierra el sistema y las bases de datos"};
    MENU mainscreen;
    setMenuData(&mainscreen, NULL, 5, 4, 1, 9, options, descriptions, &mainScreenUI, user);

    Funciones mainFuncs[] = {
        (void *)&manageUsers,
        (void *)&manageRoutes,
        (void *)&queryLog,
        (void *)&registerNextRoute,
        (void *)&checkIn,
        (void *)&checkOut,
        (void *)&DebugData};

    while (1)
    {
        focusMenu(&mainscreen);
        if (mainscreen.selected == 7)
            break;

        if (mainscreen.selected == 8)
            exit(EXIT_SUCCESS);

        if (mainscreen.selected < 0 || mainscreen.selected > 8)
            continue;

        mainFuncs[mainscreen.selected](user);
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
            break;
        }
        case USER_NOT_FOUND:
        {
            free(username.text);
            free(password.text);
            printMessage(L"Usuario no encontrado");
            break;
        }
        case INCORRECT_PASSWORD:
        {
            free(username.text);
            free(password.text);
            printMessage(L"Contraseña incorrecta");
            break;
        }
        case USER_DISABLED:
        {
            free(username.text);
            free(password.text);
            printMessage(L"Usuario deshabilitado");
            break;
        }
        }
    }
    return 1;
}