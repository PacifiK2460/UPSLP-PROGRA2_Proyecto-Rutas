#include "logic.h"
#include <stdlib.h>
#include <wchar.h>

Route *selectRoute(LList *route_source, wchar_t *title, wchar_t *subtitle,
                   Funciones __before, void *__before_args) {

  Result nroutes;
  if (route_source == NULL) {
    nroutes = number_of_routes();
    if (nroutes.Error_state != OK) {
      printMessage(L"Ocurrio un error desconocido");
      return NULL;
    }
  } else {
    nroutes.Result = (void *)llist_length(route_source);
  }

  MENU menu;
  wchar_t opciones[*(int *)nroutes.Result + 1][ROUTE_NAME_MAX_LENGTH + 1];
  wchar_t descripciones[*(int *)nroutes.Result + 1]
                       [ROUTE_DESCRIPTION_MAX_LENGTH + 1];
  for (int i = 0; i < *(int *)nroutes.Result; i++) {
    Result RouteResult = query_route_by_id(i);
    if (RouteResult.Error_state != OK) {
      printMessage(L"Ocurrio un error desconocido");
      return NULL;
    }

    Route *route = (Route *)RouteResult.Result;
    wcscpy(opciones[i], route->name);
    wcscpy(descripciones[i], route->destination);
  }

  wcscpy(opciones[*(int *)nroutes.Result + 1], title);
  wcscpy(descripciones[*(int *)nroutes.Result + 1], subtitle);

  setMenuData(&menu, NULL, 5, 4, 1, *(int *)nroutes.Result,
              (wchar_t **)opciones, (wchar_t **)descripciones, __before,
              __before_args);

  focusMenu(&menu);

  return (Route *)query_route_by_id(menu.selected).Result;
}

User *selectUser(User *requester, Funciones __before, void *__before_args) {
  int nusers = 0;

  Result nUsers = number_of_users();
  if (nUsers.Error_state != OK) {
    printMessage(L"Ocurrio un error desconocido");
    return NULL;
  }

  wchar_t usuarios[nusers][USERNAME_MAX_LENGTH + 1];
  wchar_t descrpciones[nusers][USERNAME_MAX_LENGTH + 1];

  User *user;

  for (int i = 0; i < nusers; i++) {
    Result query = query_user_by_id(*requester, i);
    if (query.Error_state != OK) {
      printMessage(L"Ocurrio un error desconocido");
      return NULL;
    }

    user = (User *)query.Result;
    wcscpy(usuarios[i], user->name);
    if (user->type == PASSANGER) {
      wcscpy(descrpciones[i], L"Pasajero");
    } else if (user->type == ADMIN) {
      wcscpy(descrpciones[i], L"Administrador");
    } else {
      wcscpy(descrpciones[i], L"Privilegios desconocidos");
    }
  }

  MENU users;
  setMenuData(&users, NULL, 5, 4, 1, nusers, (wchar_t **)usuarios,
              (wchar_t **)descrpciones, __before, __before_args);

  focusMenu(&users);

  return (User *)query_user_by_id(*requester, users.selected).Result;
}

int makeHandShake(void *data) {
  Handshake *handshake = (Handshake *)data;
  handshake->text = calloc(handshake->length, sizeof(wchar_t));
  return evaluarText(handshake->text, handshake->length);
}

void help(wchar_t *text) {
  printHelp(RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(
                185, 251, 192) L"↵" RESET DIM L" Escoger ",
            4, getrows(STDOUTPUT) - 2);

  winprint(STDOUTPUT, 4, 2, text);
  winprint(STDOUTPUT, 4, 3,
           DIM L"Escoge alguna actividad a realizar..." RESET "\0");
}

void createUser(void *data) {
  User requester = *(User *)data;
  Handshake username = {.length = USERNAME_MAX_LENGTH + 1, .text = NULL};

  Handshake password = {.length = PASSWORD_MAX_LENGTH + 1, .text = NULL};

  Handshake tipo = {.length = 5, .text = NULL};

  while (1) {
    if (input(L"Ingresa el nombre del nuevo usuario a crear",
              L"Nombre del nuevo usuario", (void *)&username,
              makeHandShake) == 1)
      break;

    printMessage(
        L"Por favor sigue las indicaciones para crear el nombre usuario");
  }

  while (1) {
    if (input(L"Ingresa la contraseña del nuevo usuario a crear",
              L"Contraseña del nuevo usuario", (void *)&password,
              makeHandShake) == 1)
      break;

    printMessage(
        L"Por favor sigue las indicaciones para crear la contraseña usuario");
  }

  {
    MENU tipoDeUsuario;
    wchar_t *opciones[] = {L"Pasajero", L"Administrador"};
    wchar_t *descrpciones[] = {
        L"Usuario con los minimos privilegios",
        L"Usuario con los privilegios maximos de administrados",
    };
    setMenuData(&tipoDeUsuario, NULL, 5, 4, 1, 2, opciones, descrpciones,
                (int (*)(void *)) & help,
                L"Selecciona el nivel de autoridad del usuario 🛡 " BOLD);
    focusMenu(&tipoDeUsuario);

    tipo.length = tipoDeUsuario.selected;
  }

  {
    int result = add_user(requester, username.text, password.text, tipo.length)
                     .Error_state;
    if (result == OK)
      printMessage(L"Usuario creado correctamente");
    else if (result == USER_ALREADY_EXISTS)
      printMessage(L"El usuario ya existe");
    else if (result == USER_NOT_ALLOWED)
      printMessage(
          L"No tienes los permisos necesarios para realizar esta accion");
    else
      printMessage(L"Ocurrio un error desconocido");
  }
}

void deleteUser(User *requester) {
  if (requester->type != PASSANGER) {
    printMessage(
        L"No tienes los permisos necesarios para realizar esta accion");
    return;
  }

  User *user = selectUser(requester, (int (*)(void *)) & help,
                          L"Selecciona el usuario a eliminar 🗑 " BOLD);

  if (user == NULL) {
    printMessage(L"Ocurrio un error desconocido");
    return;
  }

  remove_user(*requester, user->name);
  printMessage(L"Usuario eliminado correctamente");
}

void modifyUser(User *requester) {
  if (requester->type != PASSANGER) {
    printMessage(
        L"No tienes los permisos necesarios para realizar esta accion");
    return;
  }

  User *user = selectUser(requester, (int (*)(void *)) & help,
                          L"Selecciona el usuario a modificar 📝 " BOLD);

  if (user == NULL) {
    printMessage(L"Ocurrio un error desconocido");
    return;
  }

  wchar_t *opciones[] = {L"Modificar nombre", L"Modificar contraseña",
                         L"Modificar tipo de usuario"};
  wchar_t *descrpcion[] = {L"Modificar el nombre del usuario seleccionado",
                           L"Modificar la contraseña del usuario seleccionado",
                           L"Modificar tipo del usuario seleccionado"};
  MENU toModify;
  setMenuData(&toModify, NULL, 5, 4, 1, 3, opciones, descrpcion,
              (int (*)(void *)) & help,
              L"Selecciona el campo a modificar 🖋 " BOLD);
  focusMenu(&toModify);

  Handshake username = {.length = USERNAME_MAX_LENGTH + 1, .text = NULL};

  Handshake password = {.length = PASSWORD_MAX_LENGTH + 1, .text = NULL};
  Type newType = user->type;

  if (toModify.selected == 0) // Change Name
  {
    while (1) {
      if (input(L"Introduzca el nuevo nombre de usuario",
                L"Nuevo nombre de usuario", (void *)&username,
                makeHandShake) == 1)
        break;

      printMessage(L"El nombre de usuario no puede estar vacio");
    }
  } else if (toModify.selected == 1) // Change pass
  {
    while (1) {
      if (input(L"Introduzca la nueva contraseña", L"Nueva contraseña",
                (void *)&password, makeHandShake) == 1)
        break;

      printMessage(L"La contraseña no puede estar vacia");
    }
  } else if (toModify.selected == 2) // Change type
  {
    MENU tipoDeUsuario;
    wchar_t *opciones[] = {L"Pasajero", L"Administrador"};
    wchar_t *descrpciones[] = {
        L"Usuario con los minimos privilegios",
        L"Usuario con los privilegios maximos de administrados",
    };
    setMenuData(&tipoDeUsuario, NULL, 5, 4, 1, 2, opciones, descrpciones,
                (int (*)(void *)) & help,
                L"Selecciona el nivel de autoridad del usuario 🛡 " BOLD);
    focusMenu(&tipoDeUsuario);

    newType = tipoDeUsuario.selected;
  } else {
    printMessage(L"Ocurrio un error desconocido");
  }

  modify_user(*requester, username.text, password.text, newType);
}

void listUsers(User *requester) {
  if (requester->type != PASSANGER) {
    printMessage(
        L"No tienes los permisos necesarios para realizar esta accion");
    return;
  }

  User *user = selectUser(requester, (int (*)(void *)) & help,
                          L"Selecciona el usuario a listar 📝 " BOLD);

  assertm(1, L"Unimplemented");
}

void manageUsersUI(User *user) {
  printHelp(RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(
                185, 251, 192) L"↵" RESET DIM L" Escoger ",
            4, getrows(STDOUTPUT) - 2);

  wchar_t saludo[1024] = L"🛡  Estamos bajo tus ordenes " BOLD;
  wcscat(saludo, user->name);
  wcscat(saludo, RESET L", hoy y siempre.\0");

  winprint(STDOUTPUT, 4, 2, saludo);
  winprint(STDOUTPUT, 4, 3,
           DIM L"Escoge alguna actividad a realizar..." RESET "\0");
}

void manageUsers(User *user) {
  if (user->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  wchar_t *options[] = {L"Crear nuevo usuario", L"Eliminar usuario",
                        L"Modificar usuario", L"Enlistar usuario", L"Regresar"};
  wchar_t *descriptions[] = {
      L"Crea un usuario totalmente nuevo", L"Elimina algun usuario",
      L"Modifica la información de un usuario y/o deshabilitala",
      L"Consulta una lista detallada de todos los usuarios registrados",
      L"Regresa ak menu principal"};

  MENU menu;
  setMenuData(&menu, NULL, 5, 4, 1, 5, options, descriptions,
              (int (*)(void *)) & manageUsersUI, user);

  Funciones userman[] = {
      (void *)&createUser,
      (void *)&deleteUser,
      (void *)&modifyUser,
      (void *)&listUsers,
  };

  while (1) {
    focusMenu(&menu);
    if (menu.selected == 4)
      break;
    if (menu.selected >= 0 && menu.selected < 3)
      userman[menu.selected](user);
  }
}

void createRoute(User *user) {
  if (user->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  Handshake routerName = {ROUTE_NAME_MAX_LENGTH + 1, 0};
  Handshake routerDescription = {ROUTE_DESCRIPTION_MAX_LENGTH + 1, 0};

  while (1) {
    if (input(L"Ingresa el nombre de la nueva ruta a crear",
              L"Nombre de la ruta", (void *)&routerName, makeHandShake) == 1)
      break;

    printMessage(
        L"Por favor sigue las indicaciones para crear el nombre de la ruta");
  }

  while (1) {
    if (input(L"Ingresa una descripción de la ruta a crear",
              L"Descripción de la ruta", (void *)&routerDescription,
              makeHandShake) == 1)
      break;

    printMessage(L"Por favor sigue las indicaciones para crear la descripción "
                 L"de la ruta");
  }

  Result appendRoute = add_route(routerName.text, routerDescription.text, 1);
  if (appendRoute.Error_state != OK) {
    printMessage(L"Ocurrio un error desconocido");
  }

  Route *newRoute = (Route *)appendRoute.Result;

  while (1) {
    MENU agregarHorario;
    wchar_t *opciones[] = {L"Si", L"No"};
    wchar_t *descripciones[] = {L"Si, agregar horario",
                                L"No, no agregar horario y salir"};

    setMenuData(&agregarHorario, NULL, 5, 4, 1, 2, opciones, descripciones,
                (int (*)(void *)) & help,
                L"¿Deseas agregar un horario a la ruta? 🕐 " BOLD);
    focusMenu(&agregarHorario);

    if (agregarHorario.selected == 1)
      break;

    MENU dia;
    wchar_t *dias[] = {L"Lunes",   L"Martes", L"Miercoles", L"Jueves",
                       L"Viernes", L"Sabado", L"Domingo"};
    wchar_t *descripcionesDias[] = {
        L"Día lunes",   L"Día martes", L"Día miercoles", L"Día jueves",
        L"Día viernes", L"Día sabado", L"Día domingo"};
    setMenuData(&dia, NULL, 5, 4, 1, 7, dias, descripcionesDias,
                (int (*)(void *)) & help,
                L"Selecciona el día de la semana 📅 " BOLD);

    Weekday diaSeleccionado = dia.selected;

    Handshake horario = {ROUTE_HORARIO_MAX_LENGTH + 1, 0};
    int hour, minute;
    while (1) {
      int r = input(L"Ingresa el horario de la ruta a crear con el siguiente "
                    L"formato HH:MM siendo un horario de 24 horas",
                    L"Horario de la ruta (Eg. 15:52)", (void *)&horario,
                    makeHandShake);
      int c = swscanf(horario.text, L"%d:%d", &hour, &minute);
      if (r == 1 && c == 2)
        break;

      printMessage(
          L"Por favor sigue las indicaciones para crear el horario de la ruta");
    }

    Time time = {diaSeleccionado, hour, minute};
    llist_append(&newRoute->scheduled_times, &time);

    printMessage(L"Horario agregado correctamente");
  }

  printMessage(L"Ruta agregada correctamente");
}

void deleteRoute(User *user) {
  if (user->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  Route *route = selectRoute(
      NULL, L"Selecciona la ruta a eliminar 🚧 " BOLD,
      L"La ruta ah eliminar sera deshabilitada temporalmente",
      (int (*)(void *)) & help, L"Selecciona la ruta a eliminar 🗑 " BOLD);
  if (route == NULL) {
    printMessage(L"Error desconocido");
    return;
  }

  route->state = DISABLED;

  printMessage(L"Ruta eliminada correctamente");
}

void modifyRoute(User *user) {
  while (1) {
    // select route to modify
    if (user->type != ADMIN) {
      printMessage(L"Acceso denegado, solo los administradores pueden acceder "
                   L"a esta sección");
      return;
    }

    Route *route = selectRoute(
        NULL, L"Selecciona la ruta a modificar 🚧 " BOLD,
        L"A continuación seleccionaras la ruta a modificar permantemente",
        (int (*)(void *)) & help, L"Selecciona la ruta a modificar 🖋 " BOLD);

    while (1) { // select Time to modify
      MENU menu;
      wchar_t opciones[route->scheduled_times.size + 1]
                      [ROUTE_NAME_MAX_LENGTH + 1];
      wchar_t descripciones[route->scheduled_times.size + 1]
                           [ROUTE_DESCRIPTION_MAX_LENGTH + 1];

      wchar_t *days[] = {L"Lunes",   L"Martes", L"Miercoles", L"Jueves",
                         L"Viernes", L"Sabado", L"Domingo"};

      for (int i = 0; i < route->scheduled_times.size; i++) {
        Time *time = (Time *)llist_get(&route->scheduled_times, i);
        swprintf(opciones[i], ROUTE_NAME_MAX_LENGTH + 1, L"%s",
                 days[time->day]);
        swprintf(descripciones[i], ROUTE_DESCRIPTION_MAX_LENGTH + 1,
                 L"%02d:%02d", time->time.hour, time->time.minute);
      }

      wcscpy(opciones[route->scheduled_times.size + 1], L"Regresar");
      wcscpy(descripciones[route->scheduled_times.size + 1],
             L"Regresar al menu anterior");

      setMenuData(&menu, NULL, 5, 4, 1, route->scheduled_times.size,
                  (wchar_t **)opciones, (wchar_t **)descripciones,
                  (int (*)(void *)) & help,
                  L"Selecciona el horario a modificar 🕐 " BOLD);
      focusMenu(&menu);

      if (menu.selected == route->scheduled_times.size + 1)
        break;

      Time *time = (Time *)llist_get(&route->scheduled_times, menu.selected);

      while (1) { // select what to modify
        MENU menu;
        wchar_t *opciones[] = {L"Modificar día", L"Modificar hora",
                               L"Terminar"};
        wchar_t *descripciones[] = {L"Modificar día", L"Modificar hora",
                                    L"Terminar"};

        setMenuData(&menu, NULL, 5, 4, 1, 2, opciones, descripciones,
                    (int (*)(void *)) & help,
                    L"¿Qué deseas modificar? 🕐 " BOLD);
        focusMenu(&menu);

        if (menu.selected == 0) { // modify weekday
          MENU menu;
          wchar_t *opciones[] = {L"Lunes",   L"Martes", L"Miercoles", L"Jueves",
                                 L"Viernes", L"Sabado", L"Domingo"};
          wchar_t *descripciones[] = {
              L"Día lunes",   L"Día martes", L"Día miercoles", L"Día jueves",
              L"Día viernes", L"Día sabado", L"Día domingo"};

          setMenuData(&menu, NULL, 5, 4, 1, 7, opciones, descripciones,
                      (int (*)(void *)) & help,
                      L"Selecciona el día de la semana 📆" BOLD);
          focusMenu(&menu);

          time->day = menu.selected;
        } else if (menu.selected == 1) { // modigy hour
          while (1) {
            Handshake _time = {ROUTE_HORARIO_MAX_LENGTH, 0};
            if (input(L"Ingresa el nuevo tiempo a insertar", L"Nuevo tiempo",
                      (void *)&_time, makeHandShake) == 0) {
              printMessage(L"Ingresa un tiempo válido");
              return;
            }

            int hour, minute;
            if (swscanf(_time.text, L"%d:%d", &hour, &minute) != 2) {
              printMessage(L"Ingresa un tiempo válido");
              return;
            }

            if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
              printMessage(L"Ingresa un tiempo válido");
              return;
            }

            time->time.hour = hour;
            time->time.minute = minute;
          }
        } else {
          printMessage(L"Horario modificado correctamente");
          break;
          ;
        }
        printMessage(L"Horario modificado correctamente");
      }
    }
  }
}

void listRoutes(User *user) {
  while (1) {
    // select route to modify
    if (user->type != ADMIN) {
      printMessage(L"Acceso denegado, solo los administradores pueden acceder "
                   L"a esta sección");
      return;
    }
    Route *route =
        selectRoute(NULL, L"Selecciona una ruta a visualizar 🔍" BOLD,
                    L"Selecciona alguna ruta a inspeccionar 🕵️‍♂️",
                    (int (*)(void *)) & help,
                    L"Selecciona la ruta a inspeccionar a detalle 🖋 " BOLD);
    assertm(1, L"Unimplemented");
  }
}

void manageRoutes(User *user) {
  wchar_t *options[] = {L"Crear ruta", L"Eliminar ruta", L"Modificar ruta",
                        L"Consultar", L"Regresar"};
  wchar_t *descriptions[] = {
      L"Crea una ruta totalmente nueva", L"Elimina alguna ruta",
      L"Modifica la información de una ruta y/o deshabilitala",
      L"Consulta una lista detallada de todas las rutas registradas",
      L"Regresa al menu principal"};
  MENU menu;
  setMenuData(&menu, NULL, 5, 4, 1, 5, options, descriptions,
              (int (*)(void *)) & help,
              L"Selecciona una actividad a realizar 🌎 " BOLD);

  Funciones routeman[] = {
      (void *)&createRoute,
      (void *)&deleteRoute,
      (void *)&modifyRoute,
      (void *)&listRoutes,
  };

  while (1) {
    focusMenu(&menu);
    if (menu.selected == 4)
      break;
    if (menu.selected >= 0 && menu.selected < 3)
      routeman[menu.selected](user);
  }
}

void queryLogByUser(User *requester) {
  if (requester->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  while (1) {
    User *user =
        selectUser(requester, (int (*)(void *)) & help,
                   L"Selecciona un usuario a inspeccionar 🕵️‍♂️");

    if (user == NULL) {
      break;
    }

    selectRoute(&user->queued_routes,
                L"Selecciona una ruta a visualizar 🔍" BOLD,
                L"Selecciona alguna ruta a inspeccionar 🕵️‍♂️",
                (int (*)(void *)) & help,
                L"Selecciona la ruta a inspeccionar a detalle 🖋 " BOLD);
  }
}

void queryLogByMostRequestedRoutes(User *requester) {
  if (requester->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  while (1) {
    int nrutes = (int)(number_of_routes().Result);
    int nusers = (int)(number_of_users().Result);

    struct tmp {
      Route *route;
      int count;
    } routes[nrutes];

    for (int i = 0; i < nrutes; i++) {
      routes[i].route = (Route *)query_route_by_id(i).Result;
      routes[i].count = 0;
    }

    for (int i = 0; i < nusers; i++) {
      User *user = (User *)query_user_by_id(*requester, i).Result;

      int user_routes = llist_size(&user->queued_routes);

      for (int j = 0; j < user_routes; j++) {
        Route *route = (Route *)llist_get(&user->queued_routes, j);

        int found = 0;
        for (int k = 0; k < nrutes; k++) {
          if (routes[k].route == route) {
            routes[k].count++;
            found = 1;
            break;
          }
        }
      }
    }

    { // Sort routes by most requested
      for (int i = 0; i < nrutes; i++) {
        for (int j = 0; j < nrutes; j++) {
          if (routes[i].count > routes[j].count) {
            // swap them
            struct tmp temp = routes[i];
            routes[i] = routes[j];
            routes[j] = temp;
          }
        }
      }
    }

    MENU menu;
    wchar_t opciones[nrutes + 1][ROUTE_NAME_MAX_LENGTH + 1];
    wchar_t descripciones[nrutes + 1][ROUTE_DESCRIPTION_MAX_LENGTH + 1];

    for (int i = 0; i < nrutes; i++) {
      wcscpy(opciones[i], routes[i].route->name);
      wcscpy(descripciones[i], routes[i].route->destination);
    }

    wcscpy(opciones[nrutes], L"Regresar al menú principal");
    wcscpy(descripciones[nrutes], L"Regresar al menú principal");

    setMenuData(&menu, NULL, 5, 4, 1, nrutes, (wchar_t **)opciones,
                (wchar_t **)descripciones, (int (*)(void *)) & help,
                L"Selecciona la ruta a consultar 🚧 " BOLD);

    focusMenu(&menu);

    if (menu.selected == nrutes)
      break;

    assertm(1, "Not implemented yet");
  }
}

void queryLogByLeastRequestedRoutes(User *requester) {
  if (requester->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  while (1) {
    int nrutes = (int)(number_of_routes().Result);
    int nusers = (int)(number_of_users().Result);

    struct tmp {
      Route *route;
      int count;
    } routes[nrutes];

    for (int i = 0; i < nrutes; i++) {
      routes[i].route = (Route *)query_route_by_id(i).Result;
      routes[i].count = 0;
    }

    for (int i = 0; i < nusers; i++) {
      User *user = (User *)query_user_by_id(*requester, i).Result;

      int user_routes = llist_size(&user->queued_routes);

      for (int j = 0; j < user_routes; j++) {
        Route *route = (Route *)llist_get(&user->queued_routes, j);

        int found = 0;
        for (int k = 0; k < nrutes; k++) {
          if (routes[k].route == route) {
            routes[k].count++;
            found = 1;
            break;
          }
        }
      }
    }

    { // Sort routes by least requested
      for (int i = 0; i < nrutes; i++) {
        for (int j = 0; j < nrutes; j++) {
          if (routes[i].count < routes[j].count) {
            // swap them
            struct tmp temp = routes[i];
            routes[i] = routes[j];
            routes[j] = temp;
          }
        }
      }
    }

    MENU menu;
    wchar_t opciones[nrutes + 1][ROUTE_NAME_MAX_LENGTH + 1];
    wchar_t descripciones[nrutes + 1][ROUTE_DESCRIPTION_MAX_LENGTH + 1];

    for (int i = 0; i < nrutes; i++) {
      wcscpy(opciones[i], routes[i].route->name);
      wcscpy(descripciones[i], routes[i].route->destination);
    }

    wcscpy(opciones[nrutes], L"Regresar al menú principal");
    wcscpy(descripciones[nrutes], L"Regresar al menú principal");

    setMenuData(&menu, NULL, 5, 4, 1, nrutes, (wchar_t **)opciones,
                (wchar_t **)descripciones, (int (*)(void *)) & help,
                L"Selecciona la ruta a consultar 🚧 " BOLD);

    focusMenu(&menu);

    if (menu.selected == nrutes)
      break;

    assertm(1, "Not implemented yet");
  }
}

void queryLogByUsersWithOneRequest(User *requester) {
  if (requester->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  while (1) {
    int nusers = (int)(number_of_users().Result);
    struct stats {
      User *user;
      LList routes;
    };

    LList users;

    for (int i = 0; i < nusers; i++) {
      struct stats *newstat = calloc(1, sizeof(struct stats));
      newstat->user = (User *)query_user_by_id(*requester, i).Result;
      memset(&newstat->routes, 0, sizeof(LList));

      int user_routes = llist_size(&newstat->user->queued_routes);

      int atLeastOne = 0;
      for (int j = 0; j < user_routes; j++) {
        Route *route = (Route *)llist_get(&newstat->user->queued_routes, j);

        if (route->state == TAKEN) {
          llist_add(&newstat->routes, route);
          atLeastOne = 1;
        }
      }

      if (atLeastOne)
        llist_add(&users, newstat);
    }

    nusers = llist_size(&users);

    MENU menu;
    wchar_t opciones[nusers + 1][ROUTE_NAME_MAX_LENGTH + 1];
    wchar_t descripciones[nusers + 1][ROUTE_DESCRIPTION_MAX_LENGTH + 1];

    for (int i = 0; i < nusers; i++) {
      struct stats *user = ((struct stats *)llist_get(&users, i))->user;
      Route *route = llist_get(&user->routes, 0);

      wcscpy(opciones[i], user->user->name);
      wcscpy(descripciones[i], (route != NULL) ? route->name : L"");
    }

    wcscpy(opciones[nusers], L"Regresar al menú principal");
    wcscpy(descripciones[nusers], L"Regresar al menú principal");

    setMenuData(&menu, NULL, 5, 4, 1, nusers, (wchar_t **)opciones,
                (wchar_t **)descripciones, (int (*)(void *)) & help,
                L"Selecciona la ruta a consultar 🚧 " BOLD);

    focusMenu(&menu);

    if (menu.selected == nusers)
      break;

    assertm(1, "Not implemented yet");
  }
}

void queryLogByUsersWithNoRequests(User *requester) {
  if (requester->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  while (1) {
    int nusers = (int)(number_of_users().Result);
    struct stats {
      User *user;
      LList routes;
    };

    LList users;

    for (int i = 0; i < nusers; i++) {
      struct stats *newstat = callco(1, sizeof(struct stats));
      newstat->user = (User *)query_user_by_id(*requester, i).Result;
      memset(&newstat->routes, 0, sizeof(LList));

      int user_routes = llist_size(&newstat->user->queued_routes);

      int atLeastOne = 0;
      for (int j = 0; j < user_routes; j++) {
        Route *route = (Route *)llist_get(&newstat->user->queued_routes, j);

        if (route->state == REQUESTED) {
          llist_add(&newstat->routes, route);
          atLeastOne = 1;
        }
      }

      if (atLeastOne)
        llist_add(&users, newstat);
    }

    nusers = llist_size(&users);

    MENU menu;
    wchar_t opciones[nusers + 1][ROUTE_NAME_MAX_LENGTH + 1];
    wchar_t descripciones[nusers + 1][ROUTE_DESCRIPTION_MAX_LENGTH + 1];

    for (int i = 0; i < nusers; i++) {
      struct stats *user = ((struct stats *)llist_get(&users, i))->user;
      Route *route = llist_get(&user->routes, 0);

      wcscpy(opciones[i], user->user->name);
      wcscpy(descripciones[i], (route != NULL) ? route->name : L"");
    }

    wcscpy(opciones[nusers], L"Regresar al menú principal");
    wcscpy(descripciones[nusers], L"Regresar al menú principal");

    setMenuData(&menu, NULL, 5, 4, 1, nusers, (wchar_t **)opciones,
                (wchar_t **)descripciones, (int (*)(void *)) & help,
                L"Selecciona la ruta a consultar 🚧 " BOLD);

    focusMenu(&menu);

    if (menu.selected == nusers)
      break;

    assertm(1, "Not implemented yet");
  }
}

void queryLogByUsersWithCompletedRoutes(User *requester) {
  if (requester->type != ADMIN) {
    printMessage(L"Acceso denegado, solo los administradores pueden acceder a "
                 L"esta sección");
    return;
  }

  while (1) {
    int nusers = (int)(number_of_users().Result);
    struct stats {
      User *user;
      LList routes;
    };

    LList users;

    for (int i = 0; i < nusers; i++) {
      struct stats *newstat = callco(1, sizeof(struct stats));
      newstat->user = (User *)query_user_by_id(*requester, i).Result;
      memset(&newstat->routes, 0, sizeof(LList));

      int user_routes = llist_size(&newstat->user->queued_routes);

      int atLeastOne = 0;
      for (int j = 0; j < user_routes; j++) {
        Route *route = (Route *)llist_get(&newstat->user->queued_routes, j);

        if (route->state == COMPLETED) {
          llist_add(&newstat->routes, route);
          atLeastOne = 1;
        }
      }

      if (atLeastOne)
        llist_add(&users, newstat);
    }

    nusers = llist_size(&users);

    MENU menu;
    wchar_t opciones[nusers + 1][ROUTE_NAME_MAX_LENGTH + 1];
    wchar_t descripciones[nusers + 1][ROUTE_DESCRIPTION_MAX_LENGTH + 1];

    for (int i = 0; i < nusers; i++) {
      struct stats *user = ((struct stats *)llist_get(&users, i))->user;
      Route *route = llist_get(&user->routes, 0);

      wcscpy(opciones[i], user->user->name);
      wcscpy(descripciones[i], (route != NULL) ? route->name : L"");
    }

    wcscpy(opciones[nusers], L"Regresar al menú principal");
    wcscpy(descripciones[nusers], L"Regresar al menú principal");

    setMenuData(&menu, NULL, 5, 4, 1, nusers, (wchar_t **)opciones,
                (wchar_t **)descripciones, (int (*)(void *)) & help,
                L"Selecciona la ruta a consultar 🚧 " BOLD);

    focusMenu(&menu);

    if (menu.selected == nusers)
      break;

    assertm(1, "Not implemented yet");
  }
}

void queryLog(User *user) {
  MENU menu;
  wchar_t *opciones[] = {
      L"Consultar por usuario",
      L"Organizar por rutas más solicitadas",
      L"Organizar por rutas menos solicitadas",
      L"Listar rutas cuyos usuarios no se han bajado",
      L"Listar rutas cuyos usuarios no se han subido",
      L"Listar rutas cuyos usuarios completaron el recorrido",
      L"Regresar al menú principal"};

  wchar_t *descripciones[] = {
      L"Consulta los registros de un usuario en específico",
      L"Consulta las rutas más solicitadas",
      L"Consulta las rutas menos solicitadas",
      L"Listado de las rutas cuyos usuarios no se han bajado",
      L"Listado de las rutas cuyos usuarios no se han subido",
      L"Listado de las rutas cuyos usuarios completaron el recorrido",
      L"Regresa al menú principal"};

  setMenuData(&menu, NULL, 5, 4, 1, 7, opciones, descripciones,
              (int (*)(void *)) & help,
              L"Selecciona una actividad a realizar 📊 " BOLD);
  focusMenu(&menu);

  Funciones logman[] = {
      (void *)&queryLogByUser,
      (void *)&queryLogByMostRequestedRoutes,
      (void *)&queryLogByLeastRequestedRoutes,
      (void *)&queryLogByUsersWithOneRequest,
      (void *)&queryLogByUsersWithNoRequests,
      (void *)&queryLogByUsersWithCompletedRoutes,
  };

  if (menu.selected >= 0 && menu.selected < 6)
    logman[menu.selected](user);

  if (menu.selected == 6)
    return;
}

void registerNextRoute(User *user) {
  while (1) {
    Route *userRoute = malloc(sizeof(Route));
    if (userRoute == NULL) {
      printMessage(L"Error al crear la ruta");
      return;
    }
    Route *route = selectRoute(NULL, L"Selecciona la ruta a registrar 🚧 " BOLD,
                               L"Selecciona la proxima ruta a tomar",
                               (int (*)(void *)) & help,
                               L"Selecciona tu proxima ruta a tomar 🚧 " BOLD);
    if (route == NULL)
      return;

    memcpy(userRoute, route, sizeof(Route));

    // List available schedules
    MENU menu;
    {
      int nschedules = llist_size(&route->scheduled_times);
      wchar_t opciones[nschedules + 1][ROUTE_NAME_MAX_LENGTH + 1];
      wchar_t descripciones[nschedules + 1][ROUTE_DESCRIPTION_MAX_LENGTH + 1];
      wchar_t *days[] = {L"Lunes",   L"Martes", L"Miercoles", L"Jueves",
                         L"Viernes", L"Sabado", L"Domingo"};
      for (int i = 0; i < nschedules; i++) {
        Time *time = (Time *)llist_get(&route->scheduled_times, i);
        wcscpy(opciones[i], days[time->day]);
        // coopy formated time to description
        swprintf(descripciones[i], ROUTE_DESCRIPTION_MAX_LENGTH, L"%02d:%02d",
                 time->time.hour, time->time.minute);
      }

      wcscpy(opciones[nschedules], L"Regresar al menú principal");
      wcscpy(descripciones[nschedules], L"Regresar al menú principal");

      setMenuData(&menu, NULL, 5, 4, 1, nschedules, (wchar_t **)opciones,
                  (wchar_t **)descripciones, (int (*)(void *)) & help,
                  L"Selecciona el horario de la ruta a registrar 🚧 " BOLD);

      focusMenu(&menu);
      if (menu.selected == nschedules) {
        return;
      }
    }

    userRoute->state = REQUESTED;
    while (llist_size(&userRoute->scheduled_times) > 0) {
      llist_remove(&userRoute->scheduled_times, 0);
    }

    Time *userTime = llist_get(&route->scheduled_times, menu.selected);

    if (llist_append(&userRoute->scheduled_times, userTime) != 0) {
      printMessage(L"Error al registrar la ruta");
      continue;
    }

    if (llist_append(&user->queued_routes, userRoute) != 0) {
      printMessage(L"Error al registrar la ruta");
      continue;
    }
    printMessage(L"Ruta registrada exitosamente");
  }
}

// Remove from listing the already taken routes
void checkIn(User *user) {
  while (1) {
    Route *route = selectRoute(
        &user->queued_routes, L"Selecciona la ruta a registrar 🚧 " BOLD,
        L"Selecciona la proxima ruta a tomar", (int (*)(void *)) & help,
        L"Selecciona tu proxima ruta a tomar 🚧 " BOLD);

    if (route == NULL)
      return;

    if (route->state == COMPLETED) {
      printMessage(L"La ruta ya ha sido completada");
      continue;
    }
    if (route->state == TAKEN) {
      printMessage(L"La ruta esta en curso de completarse");
      continue;
    }

    route->state = TAKEN;

    printMessage(L"Check-in realizado exitosamente ✅");
  }
}

void checkOut(User *user) {
  while (1) {
    Route *route = selectRoute(
        &user->queued_routes, L"Selecciona la ruta a registrar 🚧 " BOLD,
        L"Selecciona la proxima ruta a tomar", (int (*)(void *)) & help,
        L"Selecciona tu proxima ruta a tomar 🚧 " BOLD);

    if (route == NULL)
      return;

    if (route->state == COMPLETED) {
      printMessage(L"La ruta ya ha sido completada");
      continue;
    }
    if (route->state == REQUESTED) {
      printMessage(L"No puedes terminar una ruta que no se ah empezado");
      continue;
    }

    route->state = COMPLETED;

    printMessage(L"Check-Out realizado exitosamente ✅");
  }
}

void DebugData(User *user) {
  while (1) {
    MENU menu;
    wchar_t *opciones[] = {
        L"Usuarios",
        L"Rutas",
        L"Regresar al menú principal",
    };

    wchar_t *descripciones[] = {
        L"Ver dirección en memoría usuarios",
        L"Ver dirección en memoría rutas",
        L"Regresar al menú principal",
    };

    setMenuData(&menu, NULL, 5, 4, 1, 3, (wchar_t **)opciones,
                (wchar_t **)descripciones, (int (*)(void *)) & help,
                L"Selecciona una opción 🚧 " BOLD);

    focusMenu(&menu);

    if (menu.selected == 3)
      return;
    int nitems = 0;

    Result n;
    if (menu.selected == 0) {
      n = number_of_users();
    } else if (menu.selected == 1) {
      n = number_of_routes();
    }

    if (n.Result != OK) {
      printMessage(L"Error al obtener el número de elementos");
      continue;
    } else {
      nitems = (int)n.Result;
    }

    wchar_t *items[nitems + 1];
    wchar_t *descriptions[nitems + 1];

    for (int i = 0; i < nitems; i++) {
      descriptions[i] = malloc(sizeof(wchar_t) * 50);
      items[i] = malloc(sizeof(wchar_t) * 50);
      void *pointer;

      if (menu.selected == 0) {
        User *tmp = query_user_by_id(*user, i).Result;
        pointer = tmp;
        swprintf(items[i], 49, L"%s", tmp->name);
      } else if (menu.selected == 1) {
        Route *tmp = query_route_by_id(i).Result;
        pointer = tmp;
        swprintf(items[i], 49, L"%s", tmp->name);
      }

      swprintf(descriptions[i], 49, ITALIC DIM L"Dirección %p" RESET, pointer);
    }

    setMenuData(&menu, NULL, 5, 4, 1, nitems, (wchar_t **)items,
                (wchar_t **)descriptions, (int (*)(void *)) & help,
                L"Selecciona una opción 🚧 " BOLD);

    focusMenu(&menu);

    for (int i = 0; i < nitems; i++) {
      free(items[i]);
      free(descriptions[i]);
    }
  }
}

void mainScreenUI(void *data) {
  printHelp(RESET FRGB(185, 251, 192) L"↕" RESET DIM L" Sig/Ant " RESET FRGB(
                185, 251, 192) L"↵" RESET DIM L" Escoger ",
            4, getrows(STDOUTPUT) - 2);

  User *user = (User *)data;
  wchar_t saludo[1024] = L"👋 Bienvenido " BOLD;
  wcscat(saludo, user->name);
  wcscat(saludo, RESET L", es un placer tenerte de vuelta.\0");

  winprint(STDOUTPUT, 4, 2, saludo);
  winprint(STDOUTPUT, 4, 3,
           DIM L"Escoge alguna actividad a realizar..." RESET "\0");
}

void mainScreen(User *user) {
  if (user->type == ADMIN) {
    wint_t opcion;
    wchar_t *options[] = {L"Administrar Usuarios",  L"Administrar Rutas",
                          L"Enlistar Estadisticas", L"Debug Data",
                          L"Cerrar Sesión",         L"Cerrar Aplicación"};
    wchar_t *descriptions[] = {
        L"Agrega, elimina, u modifica los usuarios registrados",
        L"Agrega, elimina, u modifica las rutas registradas",
        L"Un completo resumen de las estadisticas del sistema",
        L"Información detallada de la información registrada",
        L"Cierra sesión para que otro usuario pueda usar el sitema",
        L"Cierra el sistema y las bases de datos"};
    MENU mainscreen;
    setMenuData(&mainscreen, NULL, 5, 4, 1, 9, options, descriptions,
                (int (*)(void *)) & mainScreenUI, user);

    Funciones mainFuncs[] = {(void *)&manageUsers, (void *)&manageRoutes,
                             (void *)&queryLog, (void *)&DebugData};

    while (1) {
      focusMenu(&mainscreen);
      if (mainscreen.selected == 4)
        break;

      if (mainscreen.selected == 5)
        exit(EXIT_SUCCESS);

      if (mainscreen.selected < 0 || mainscreen.selected > 2)
        continue;

      mainFuncs[mainscreen.selected](user);
    }
  } else {
    wint_t opcion;
    wchar_t *options[] = {L"Registrar proxima ruta", L"Registrar entrada",
                          L"Registrar salida", L"Cerrar Sesión",
                          L"Cerrar Aplicación"};
    wchar_t *descriptions[] = {
        L"Registra la ruta que vas a realizar",
        L"Registra la entrada a un lugar", L"Registra la salida de un lugar",
        L"Cierra sesión para que otro usuario pueda usar el sitema",
        L"Cierra el sistema y las bases de datos"};
    MENU mainscreen;
    setMenuData(&mainscreen, NULL, 5, 4, 1, 9, options, descriptions,
                (int (*)(void *)) & mainScreenUI, user);

    Funciones mainFuncs[] = {(void *)&registerNextRoute, (void *)&checkIn,
                             (void *)&checkOut};

    while (1) {
      focusMenu(&mainscreen);
      if (mainscreen.selected == 3)
        break;

      if (mainscreen.selected == 4)
        exit(EXIT_SUCCESS);

      if (mainscreen.selected < 0 || mainscreen.selected > 2)
        continue;

      mainFuncs[mainscreen.selected](user);
    }
  }
}

int TuiLogin() {
  while (1) {
    wprintf(CLEAR);
    Handshake username = {.length = USERNAME_MAX_LENGTH + 1, .text = NULL};

    Handshake password = {.length = PASSWORD_MAX_LENGTH + 1, .text = NULL};

    if (input(L"Ingresa tu usuario", L"Nombre de Usuario", (void *)&username,
              makeHandShake) == 0)
      return 0;

    if (input(L"Ingresa tu contraseña", L"Contraseña", (void *)&password,
              makeHandShake) == 0)
      return 0;

    Result loginAttempt = login(username.text, password.text);

    switch (loginAttempt.Error_state) {
    case OK: {
      free(username.text);
      free(password.text);
      mainScreen(loginAttempt.Result);
      break;
    }
    case USER_NOT_FOUND: {
      free(username.text);
      free(password.text);
      printMessage(L"Usuario no encontrado");
      break;
    }
    case INCORRECT_PASSWORD: {
      free(username.text);
      free(password.text);
      printMessage(L"Contraseña incorrecta");
      break;
    }
    case USER_DISABLED: {
      free(username.text);
      free(password.text);
      printMessage(L"Usuario deshabilitado");
      break;
    }
    }
  }
  return 1;
}