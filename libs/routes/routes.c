#include "routes.h"

#define ROUTES_FILE "routes.db"

static LList routes;

void freeRoutes()
{
    while (llist_size(&routes) > 0)
    {
        Route *route = llist_remove(&routes, 0);
        free(route->name);
        free(route->destination);
        while(llist_size(&route->scheduled_times) > 0){
            Time* time = llist_remove(&route->scheduled_times, 0);
            free(time);
        }
        free(route);
    }
}

Result loadAllRoutes()
{
    Result result = {OK, NULL};
    result.Error_state = OK;


    FILE *file = fopen(ROUTES_FILE, "r, ccs=UTF-8");
    if (file == 0)
    {
        // check for errors produced by fopen
        if (errno == ENOENT)
        {
            // if the file was not found, create it
            file = fopen(ROUTES_FILE, "a+, ccs=UTF-8");
            if (file == 0)
            {
                if (errno == EACCES)
                {
                    result.Error_state = FILE_PERMISSION_DENIED;
                }
                else
                {
                    result.Error_state = FOPEN_FAULT;
                }
            }
            else
            {
                result.Error_state = OK;
                // Because the file was created, we need to set the number of routes to 0
                fwprintf(file, L"0\n");
            }
        }
        else if (errno == EACCES)
        {
            result.Error_state = FILE_PERMISSION_DENIED;
        }
        else
        {
            result.Error_state = FILE_OPEN_ERROR;
        }

        // We were able to open the file, but there was an error.
        if (result.Error_state != OK)
        {
            freeRoutes();
            fclose(file);
            return result;
        }
    }

    int number_of_routes = 0;
    if (fwscanf(file, L" %d ", &number_of_routes) != 1)
    {
        result.Error_state = FILE_READ_ERROR;
        freeRoutes();
        fclose(file);
        return result;
    }
    if (fwscanf(file, L"\n") != 0)
    {
        result.Error_state = FILE_READ_ERROR;
        freeRoutes();
        fclose(file);
        return result;
    }

    for (int i = 0; i < number_of_routes; i++)
    {
        Route *route = malloc(sizeof(Route));
        if (route == 0)
        {
            result.Error_state = MALLOC_FAULT;
            freeRoutes();
            fclose(file);
            return result;
        }

        route->name = malloc(sizeof(wchar_t) * 100);
        if (route->name == 0)
        {
            result.Error_state = MALLOC_FAULT;
            freeRoutes();
            fclose(file);
            return result;
        }
        if (fwscanf(file, L"%s", route->name) != 1)
        {
            result.Error_state = FILE_READ_ERROR;
            freeRoutes();
            fclose(file);
            return result;
        }

        route->destination = malloc(sizeof(wchar_t) * 100);
        if (route->destination == 0)
        {
            result.Error_state = MALLOC_FAULT;
            freeRoutes();
            fclose(file);
            return result;
        }
        if (fwscanf(file, L"%s", route->destination) != 1)
        {
            result.Error_state = FILE_READ_ERROR;
            freeRoutes();
            fclose(file);
            return result;
        }

        int number_of_scheduled_times = 0;
        if (fwscanf(file, L"%d", &number_of_scheduled_times) != 1)
        {
            result.Error_state = FILE_READ_ERROR;
            freeRoutes();
            fclose(file);
            return result;
        }

        for (int j = 0; j < number_of_scheduled_times; j++)
        {
            Time *time = malloc(sizeof(Time));
            if (time == 0)
            {
                result.Error_state = MALLOC_FAULT;
                freeRoutes();
                fclose(file);
                return result;
            }

            if (fwscanf(file, L"%d %d:%d", &time->day,&time->time.hour, &time->time.minute) != 2)
            {
                result.Error_state = FILE_READ_ERROR;
                freeRoutes();
                fclose(file);
                return result;
            }
            llist_append(&route->scheduled_times, time);
        }

        if (fwscanf(file, L"\n") != 0)
        {
            result.Error_state = FILE_READ_ERROR;
            freeRoutes();
            fclose(file);
            return result;
        }

        llist_append(&routes, route);
    }
    fclose(file);
    return result;
}

Result add_route(wchar_t* name, wchar_t* destination, int state){
    Result result = {OK, NULL};
    result.Error_state = OK;

    Route *route = malloc(sizeof(Route));
    if (route == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    route->name = malloc(sizeof(wchar_t) * ROUTE_NAME_MAX_LENGTH+1);
    if (route->name == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    wcscpy(route->name, name);

    route->destination = malloc(sizeof(wchar_t) * ROUTE_DESCRIPTION_MAX_LENGTH+1);
    if (route->destination == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    wcscpy(route->destination, destination);

    route->state = state;

    result.Result = route;

    llist_append(&routes, route);

    return result;
}

Result number_of_routes(){
    Result result = {OK, NULL};
    result.Error_state = OK;

    int *number_of_routes = malloc(sizeof(int));
    if (number_of_routes == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    *number_of_routes = llist_size(&routes);

    result.Result = number_of_routes;
    return result;
}

Result query_route_by_id(int id){
    Result result = {OK, NULL};
    result.Error_state = OK;

    result.Result = llist_get(&routes, id);

    return result;
}