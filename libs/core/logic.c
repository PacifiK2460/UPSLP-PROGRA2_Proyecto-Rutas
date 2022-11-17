#include "logic.h"

int makeHandShake(void *data)
{
    Handshake *handshake = (Handshake *)data;
    handshake->text = calloc(handshake->length, sizeof(wchar_t));
    return evaluarText(handshake->text, handshake->length);
}

void help(wchar_t *text)
{
    printHelp(
        RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(185, 251, 192) L"↵" RESET DIM L" Escoger ",
        4, getrows(STDOUTPUT) - 2);

    winprint(STDOUTPUT, 4, 2, text);
    winprint(STDOUTPUT, 4, 3, DIM L"Escoge alguna actividad a realizar..." RESET "\0");
}

void createUser(void *data)
{
    User requester = *(User *)data;
    Handshake username = {
        .length = USERNAME_MAX_LENGTH + 1,
        .text = NULL};

    Handshake password = {
        .length = PASSWORD_MAX_LENGTH + 1,
        .text = NULL};

    Handshake tipo = {
        .length = 5,
        .text = NULL};

    while (1)
    {
        if (input(L"Ingresa el nombre del nuevo usuario a crear", L"Nombre del nuevo usuario", (void *)&username, makeHandShake) == 1)
            break;

        printMessage(L"Por favor sigue las indicaciones para crear el nombre usuario");
    }

    while (1)
    {
        if (input(L"Ingresa la contraseña del nuevo usuario a crear", L"Contraseña del nuevo usuario", (void *)&password, makeHandShake) == 1)
            break;

        printMessage(L"Por favor sigue las indicaciones para crear la contraseña usuario");
    }

    {
        MENU tipoDeUsuario;
        wchar_t *opciones[] = {L"Pasajero", L"Administrador"};
        wchar_t *descrpciones[] = {
            L"Usuario con los minimos privilegios",
            L"Usuario con los privilegios maximos de administrados",
        };
        setMenuData(&tipoDeUsuario, NULL, 5, 4, 1, 2, opciones, descrpciones, (int (*)(void *)) & help, L"Selecciona el nivel de autoridad del usuario 🛡 " BOLD);
        focusMenu(&tipoDeUsuario);

        tipo.length = tipoDeUsuario.selected;
    }

    {
        int result = add_user(requester, username.text, password.text, tipo.length).Error_state;
        if (result == OK)
            printMessage(L"Usuario creado correctamente");
        else if (result == USER_ALREADY_EXISTS)
            printMessage(L"El usuario ya existe");
        else if (result == USER_NOT_ALLOWED)
            printMessage(L"No tienes los permisos necesarios para realizar esta accion");
        else
            printMessage(L"Ocurrio un error desconocido");
    }
}

void deleteUser(User *requester)
{
    if (requester->type != PASSANGER)
    {
        printMessage(L"No tienes los permisos necesarios para realizar esta accion");
        return;
    }

    Result nUsers = number_of_users();
    int *numberOfUsers = 0;
    if (nUsers.Error_state == OK)
    {
        numberOfUsers = (int *)nUsers.Result;
    }
    wchar_t usuarios[*numberOfUsers][USERNAME_MAX_LENGTH + 1];
    wchar_t descrpciones[*numberOfUsers][USERNAME_MAX_LENGTH + 1];

    User *user;

    for (int i = 0; i < *numberOfUsers; i++)
    {
        Result query = query_user_by_id(*requester, i);
        if (query.Error_state != OK)
        {
            printMessage(L"Ocurrio un error desconocido");
            return;
        }

        user = (User *)query.Result;
        wcscpy(usuarios[i], user->name);
        if (user->type == PASSANGER)
        {
            wcscpy(descrpciones[i], L"Pasajero");
        }
        else if (user->type == ADMIN)
        {
            wcscpy(descrpciones[i], L"Administrador");
        }
        else
        {
            wcscpy(descrpciones[i], L"Privilegios desconocidos");
        }
    }

    MENU users;
    setMenuData(&users, NULL, 5, 4, 1, *numberOfUsers, (wchar_t **)usuarios, (wchar_t **)descrpciones, (int (*)(void *)) & help, L"Selecciona el usuario a eliminar 🧽 " BOLD);
    focusMenu(&users);
    {
        Result query = query_user_by_id(*requester, users.selected);
        if (query.Error_state != OK)
        {
            printMessage(L"Ocurrio un error desconocido");
            return;
        }

        user = (User *)query.Result;
    }
    remove_user(*requester, user->name);
    free(numberOfUsers);
    printMessage(L"Usuario eliminado correctamente");
}

void modifyUser(User *requester)
{
    if (requester->type != PASSANGER)
    {
        printMessage(L"No tienes los permisos necesarios para realizar esta accion");
        return;
    }

    Result nUsers = number_of_users();
    int *numberOfUsers = 0;
    if (nUsers.Error_state == OK)
    {
        numberOfUsers = (int *)nUsers.Result;
    }
    wchar_t usuarios[*numberOfUsers][USERNAME_MAX_LENGTH + 1];
    wchar_t descrpciones[*numberOfUsers][USERNAME_MAX_LENGTH + 1];

    User *user;

    for (int i = 0; i < *numberOfUsers; i++)
    {
        Result query = query_user_by_id(*requester, i);
        if (query.Error_state != OK)
        {
            printMessage(L"Ocurrio un error desconocido");
            return;
        }

        user = (User *)query.Result;
        wcscpy(usuarios[i], user->name);
        if (user->type == PASSANGER)
        {
            wcscpy(descrpciones[i], L"Pasajero");
        }
        else if (user->type == ADMIN)
        {
            wcscpy(descrpciones[i], L"Administrador");
        }
        else
        {
            wcscpy(descrpciones[i], L"Privilegios desconocidos");
        }
    }

    MENU users;
    setMenuData(&users, NULL, 5, 4, 1, *numberOfUsers, (wchar_t **)usuarios, (wchar_t **)descrpciones, (int (*)(void *)) & help, L"Selecciona el usuario a modificar 🖋 " BOLD);
    focusMenu(&users);

    wchar_t *opciones[] = {L"Modificar nombre", L"Modificar contraseña", L"Modificar tipo de usuario"};
    wchar_t *descrpcion[] = {L"Modificar el nombre del usuario seleccionado", L"Modificar la contraseña del usuario seleccionado", L"Modificar tipo del usuario seleccionado"};
    MENU toModify;
    setMenuData(&toModify, NULL, 5, 4, 1, 3, opciones, descrpcion, (int (*)(void *)) & help, L"Selecciona el campo a modificar 🖋 " BOLD);
    focusMenu(&toModify);

    { // User to modify
        Result query = query_user_by_id(*requester, toModify.selected);
        if (query.Error_state != OK)
        {
            printMessage(L"Ocurrio un error desconocido");
            return;
        }

        user = (User *)query.Result;
    }

    Handshake username = {
        .length = USERNAME_MAX_LENGTH + 1,
        .text = NULL};

    Handshake password = {
        .length = PASSWORD_MAX_LENGTH + 1,
        .text = NULL};
    Type newType = user->type;

    if (toModify.selected == 0) // Change Name
    {
        while (1)
        {
            if (input(L"Introduzca el nuevo nombre de usuario", L"Nuevo nombre de usuario", (void *)&username, makeHandShake) == 1)
                break;

            printMessage(L"El nombre de usuario no puede estar vacio");
        }
    }
    else if (toModify.selected == 1) // Change pass
    {
        while (1)
        {
            if (input(L"Introduzca la nueva contraseña", L"Nueva contraseña", (void *)&password, makeHandShake) == 1)
                break;

            printMessage(L"La contraseña no puede estar vacia");
        }
    }
    else if (toModify.selected == 2) // Change type
    {
        MENU tipoDeUsuario;
        wchar_t *opciones[] = {L"Pasajero", L"Administrador"};
        wchar_t *descrpciones[] = {
            L"Usuario con los minimos privilegios",
            L"Usuario con los privilegios maximos de administrados",
        };
        setMenuData(&tipoDeUsuario, NULL, 5, 4, 1, 2, opciones, descrpciones, (int (*)(void *)) & help, L"Selecciona el nivel de autoridad del usuario 🛡 " BOLD);
        focusMenu(&tipoDeUsuario);

        newType = tipoDeUsuario.selected;
    }
    else
    {
        printMessage(L"Ocurrio un error desconocido");
    }

    modify_user(*requester, username.text, password.text, newType);
}

void listUsers(User *requester)
{
    User *user;
    { // select user
        Result nUsers = number_of_users();
        int *numberOfUsers = 0;
        if (nUsers.Error_state == OK)
        {
            numberOfUsers = (int *)nUsers.Result;
        }
        wchar_t usuarios[*numberOfUsers][USERNAME_MAX_LENGTH + 1];
        wchar_t descrpciones[*numberOfUsers][USERNAME_MAX_LENGTH + 1];

        for (int i = 0; i < *numberOfUsers; i++)
        {
            Result query = query_user_by_id(*requester, i);
            if (query.Error_state != OK)
            {
                printMessage(L"Ocurrio un error desconocido");
                return;
            }

            user = (User *)query.Result;
            wcscpy(usuarios[i], user->name);
            if (user->type == PASSANGER)
            {
                wcscpy(descrpciones[i], L"Pasajero");
            }
            else if (user->type == ADMIN)
            {
                wcscpy(descrpciones[i], L"Administrador");
            }
            else
            {
                wcscpy(descrpciones[i], L"Privilegios desconocidos");
            }
        }

        MENU users;
        setMenuData(&users, NULL, 5, 4, 1, *numberOfUsers, (wchar_t **)usuarios, (wchar_t **)descrpciones, (int (*)(void *)) & help, L"Selecciona el usuario a consultar 🔍 " BOLD);
        focusMenu(&users);

        user = (User *)query_user_by_id(*requester, users.selected).Result;
    }

    { // Print user info
    }
}

void manageUsersUI(User *user)
{
    printHelp(
        RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(185, 251, 192) L"↵" RESET DIM L" Escoger ",
        4, getrows(STDOUTPUT) - 2);

    wchar_t saludo[1024] = L"🛡  Estamos bajo tus ordenes " BOLD;
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
    setMenuData(&menu, NULL, 5, 4, 1, 5, options, descriptions, (int (*)(void *)) & manageUsersUI, user);

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
        if (menu.selected >= 0 && menu.selected < 3)
            userman[menu.selected](user);
    }
}

void createRoute(User *user)
{
    if(user->type != ADMIN)
    {
        printMessage(L"Acceso denegado, solo los administradores pueden acceder a esta sección");
        return;
    }

    Handshake routerName = {ROUTE_NAME_MAX_LENGTH + 1, 0};
    Handshake routerDescription = {ROUTE_DESCRIPTION_MAX_LENGTH + 1, 0};

    while (1)
    {
        if (input(L"Ingresa el nombre de la nueva ruta a crear", L"Nombre de la ruta", (void *)&routerName, makeHandShake) == 1)
            break;

        printMessage(L"Por favor sigue las indicaciones para crear el nombre de la ruta");
    }

    while (1)
    {
        if (input(L"Ingresa una descripción de la ruta a crear", L"Descripción de la ruta", (void *)&routerDescription, makeHandShake) == 1)
            break;

        printMessage(L"Por favor sigue las indicaciones para crear la descripción de la ruta");
    }

    Result appendRoute = add_route(routerName.text, routerDescription.text, 1);
    if(appendRoute.Error_state != OK)
    {
        printMessage(L"Ocurrio un error desconocido");
    }

    Route *newRoute = (Route*)appendRoute.Result;

    while (1)
    {
        MENU agregarHorario;
        wchar_t *opciones[] = {L"Si", L"No"};
        wchar_t *descripciones[] = {L"Si, agregar horario", L"No, no agregar horario y salir"};

        setMenuData(&agregarHorario, NULL, 5, 4, 1, 2, opciones, descripciones, (int (*)(void *)) & help, L"¿Deseas agregar un horario a la ruta? 🕐 " BOLD);
        focusMenu(&agregarHorario);

        if (agregarHorario.selected == 1)
            break;

        MENU dia;
        wchar_t *dias[] = {L"Lunes", L"Martes", L"Miercoles", L"Jueves", L"Viernes", L"Sabado", L"Domingo"};
        wchar_t *descripcionesDias[] = {L"Día lunes", L"Día martes", L"Día miercoles", L"Día jueves", L"Día viernes", L"Día sabado", L"Día domingo"};
        setMenuData(&dia, NULL, 5, 4, 1, 7, dias, descripcionesDias, (int (*)(void *)) & help, L"Selecciona el día de la semana 📅 " BOLD);

        Weekday diaSeleccionado = dia.selected;

        Handshake horario = {ROUTE_HORARIO_MAX_LENGTH+ 1, 0};
        int hour, minute;
        while (1)
        {
            int r = input(L"Ingresa el horario de la ruta a crear con el siguiente formato HH:MM siendo un horario de 24 horas", L"Horario de la ruta (Eg. 15:52)", (void *)&horario, makeHandShake);
            int c = swscanf(horario.text, L"%d:%d", &hour, &minute);
            if (r == 1 && c == 2)
                break;

            printMessage(L"Por favor sigue las indicaciones para crear el horario de la ruta");
        }

        Time time = {diaSeleccionado, hour, minute};
        llist_append(&newRoute->scheduled_times, &time);

        printMessage(L"Horario agregado correctamente");
    }

    printMessage(L"Ruta agregada correctamente");   
}

void deleteRoute(User *user) {
    if(user->type != ADMIN)
    {
        printMessage(L"Acceso denegado, solo los administradores pueden acceder a esta sección");
        return;
    }
    Result nroutes = number_of_routes();
    if(nroutes.Error_state != OK)
    {
        printMessage(L"Ocurrio un error desconocido");
        return;
    }

    MENU menu;
    wchar_t opciones[*(int*)nroutes.Result][ROUTE_NAME_MAX_LENGTH + 1];
    wchar_t descripciones[*(int*)nroutes.Result][ROUTE_DESCRIPTION_MAX_LENGTH + 1];
    Route *route;
    for(int i = 0; i < *(int*)nroutes.Result; i++)
    {
        Result RouteResult = query_route_by_id(i);
        if(RouteResult.Error_state != OK)
        {
            printMessage(L"Ocurrio un error desconocido");
            return;
        }

        route = (Route*)RouteResult.Result;
        wcscpy(opciones[i], route->name);
        wcscpy(descripciones[i], route->destination);
    }

    setMenuData(&menu, NULL, 5, 4, 1, *(int*)nroutes.Result, (wchar_t**)opciones, (wchar_t**)descripciones, (int (*)(void *)) & help, L"Selecciona la ruta a eliminar 🚧 " BOLD);
    focusMenu(&menu);

    route = (Route*)query_route_by_id(menu.selected).Result;

    route->state = DISABLED;

    printMessage(L"Ruta eliminada correctamente");
}

void modifyRoute(User *user) {}

void listRoutes(User *user) {}

void manageRoutes(User *user)
{
    wchar_t *options[] = {L"Crear ruta", L"Eliminar ruta", L"Modificar ruta", L"Consultar", L"Regresar"};
    wchar_t *descriptions[] = {L"Crea una ruta totalmente nueva", L"Elimina alguna ruta", L"Modifica la información de una ruta y/o deshabilitala", L"Consulta una lista detallada de todas las rutas registradas", L"Regresa al menu principal"};
    MENU menu;
    setMenuData(&menu, NULL, 5, 4, 1, 5, options, descriptions, (int (*)(void *)) & help, L"Selecciona una actividad a realizar 🌎 " BOLD);

    Funciones routeman[] = {
        (void *)&createRoute,
        (void *)&deleteRoute,
        (void *)&modifyRoute,
        (void *)&listRoutes,
    };

    while (1)
    {
        focusMenu(&menu);
        if (menu.selected == 4)
            break;
        if (menu.selected >= 0 && menu.selected < 3)
            routeman[menu.selected](user);
    }
}

void queryLog(User *user) {}

void registerNextRoute(User *user) {}

void checkIn(User *user) {}

void checkOut(User *user) {}

void DebugData(User *user) {}

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
    setMenuData(&mainscreen, NULL, 5, 4, 1, 9, options, descriptions, (int (*)(void *)) & mainScreenUI, user);

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