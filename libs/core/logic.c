#include "logic.h"

int makeHandShake(void *data)
{
    Handshake *handshake = (Handshake *)data;
    handshake->text = calloc(handshake->length, sizeof(wchar_t));
    return evaluarText(handshake->text, handshake->length);
}

void createUser(void *data)
{
    User *newUser = calloc(1, sizeof(User));

    Handshake username = {
        .length = USERNAME_MAX_LENGTH + 1,
        .text = NULL};

    Handshake password = {
        .length = PASSWORD_MAX_LENGTH + 1,
        .text = NULL};

    Handshake tipo = {
        .length = 5,
        .text = NULL};

    Handshake estado = {
        .length = 5,
        .text = NULL};

    while (1)
    {
        if (input(L"Ingresa el nombre del nuevo usuario a crear", L"Nombre del nuevo usuario", (void *)&username, makeHandShake) != 0)
            continue;

        printMessage(L"Por favor sigue las indicaciones para crear el nombre usuario");
    }

    while (1)
    {
        if (input(L"Ingresa la contraseña del nuevo usuario a crear", L"Contraseña del nuevo usuario", (void *)&password, makeHandShake) != 0)
            continue;

        printMessage(L"Por favor sigue las indicaciones para crear la contraseña usuario");
    }

    while (1)
    {
        if (input(L"Ingresa los priilegios", L"Privilegios del nuevo usuario", (void *)&password, makeHandShake) != 0)
            continue;

        printMessage(L"Por favor sigue las indicaciones para crear la contraseña usuario");
    }
}

void deleteUser(void *data)
{
}

void modifyUser(void *data)
{
}

void listUsers(void *data)
{
}

void manageUsersUI(User *user)
{
    printHelp(
        RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(185, 251, 192) L"↵" RESET DIM L" Escoger ",
        4, getrows(STDOUTPUT) - 2);

    wchar_t saludo[1024] = L"🛡 Estamos bajo tus ordenes " BOLD;
    wcscat(saludo, user->name);
    wcscat(saludo, RESET L", hoy y siempre.\0");

    winprint(STDOUTPUT, 4, 2, saludo);
    winprint(STDOUTPUT, 4, 3, DIM L"Escoge alguna actividad a realizar..." RESET "\0");
}

void manageUsers(User *user)
{
    if (user->type != ADMIN)
    {
        printMessage(L"Acceso denegado, solo los administradores pueden acceder a esta sección");
        return;
    }

    wchar_t *options[] = {L"Crear nuevo usuario", L"Eliminar usuario", L"Modificar usuario", L"Enlistar usuario", L"Regresar"};
    wchar_t *descriptions[] = {L"Crea un usuario totalmente nuevo", L"Elimina algun usuario", L"Modifica la información de un usuario y/o deshabilitala", L"Consulta una lista detallada de todos los usuarios registrados", L"Regresa ak menu principal"};

    MENU menu;
    setMenuData(&menu, NULL, 5, 4, 1, 5, options, descriptions, &manageUsersUI, (void *)user);

    Funciones userman[] = {
        (void *)&createUser,
        (void *)&deleteUser,
        (void *)&modifyUser,
        (void *)&listUsers,
    };

    while (1)
    {
        focusMenu(&menu);
        if (menu.selected == 4)
            break;
        if (menu.selected > 0 && menu.selected < 5)
            userman[menu.selected - 1](&user);
    }
}

void manageRoutes(User user) {}
void queryLog(User user) {}
void registerNextRoute(User user) {}
void checkIn(User user) {}
void checkOut(User user) {}
void DebugData(User user) {}

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
    wchar_t *options[] = {L"Administrar Usuarios", L"Administrar Rutas", L"Enlistar Estadisticas", L"Reservar Asiento", L"Check In", L"Check Out", L"Debug Data", L"Cerrar Sesión", L"Cerrar Aplicación"};
    wchar_t *descriptions[] = {L"Agrega, elimina, u modifica los usuarios registrados", L"Agrega, elimina, u modifica las rutas registradas", L"Un completo resumen de las estadisticas del sistema", L"Reserva el asiento de tu siguiente parada", L"Tomar registro de toma de ruta", L"Terminar ruta", L"Información detallada de la información registrada", L"Cierra sesión para que otro usuario pueda usar el sitema", L"Cierra el sistema y las bases de datos"};
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