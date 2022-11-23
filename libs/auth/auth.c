#include "auth.h"

#define USERS_FILE "users.db"

static LList users;

void freeUsers()
{
    while (llist_size(&users) > 0)
    {
        User *user = llist_remove(&users, 0);
        // free(user->name);
        // free(user->pass);

        if (user->type == PASSANGER)
        {
            freeUserRoutes(*user);
        }

        free(user);
    }
    return;
}

void freeUserRoutes(User user)
{
    while (llist_size(&user.queued_routes) > 0)
    {
        Route *route = llist_remove(&user.queued_routes, 0);
        free(route->name);
        free(route->destination);
        while (llist_size(&route->scheduled_times) > 0)
        {
            Time *time = llist_remove(&route->scheduled_times, 0);
            free(time);
        }
        free(route);
    }
}

Result login(const wchar_t *name, const wchar_t *pass)
{
    Result result = {OK, NULL};

    for (int i = 0; i < llist_size(&users); i++)
    {
        User *user = llist_get(&users, i);
        if (wcscmp(user->name, name) == 0)
        {
            if (wcscmp(user->pass, pass) == 0)
            {
                if (user->state == ENABLED)
                {
                    result.Error_state = OK;
                    result.Result = user;
                    return result;
                }
                else
                {
                    result.Error_state = USER_DISABLED;
                    return result;
                }
            }
            else
            {
                result.Error_state = INCORRECT_PASSWORD;
                return result;
            }
        }
    }

    result.Error_state = USER_NOT_FOUND;
    return result;
}

Result query_user(const User Requester, const wchar_t *UserName)
{
    Result result = {OK, NULL};

    if (Requester.type != ADMIN)
    {
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for (int i = 0; i < llist_size(&users); i++)
    {
        User *user = llist_get(&users, i);
        if (wcscmp(user->name, UserName) == 0)
        {
            result.Error_state = OK;
            result.Result = user;
            return result;
        }
    }

    result.Error_state = USER_NOT_FOUND;
    return result;
}

Result number_of_users()
{
    Result result = {OK, NULL};
    result.Result = malloc(sizeof(int));
    if (result.Result == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    int *num = calloc(1, sizeof(int));
    *num = llist_size(&users);
    result.Result = num;
    return result;
}

Result query_user_by_id(const User Requester, const int id)
{
    Result res = {OK, NULL};
    if (Requester.type != ADMIN)
    {
        res.Error_state = USER_NOT_ALLOWED;
        return res;
    }

    if (id < 0 || id >= llist_size(&users))
    {
        res.Error_state = USER_NOT_FOUND;
        return res;
    }

    res.Result = llist_get(&users, id);
    return res;
}

Result loadAllUsers()
{
    memset(&users, 0, sizeof(LList));
    Result result = {OK, NULL};

    FILE *file = fopen(USERS_FILE, "r, ccs=UTF-8");
    if (file == 0)
    {
        // check for errors produced by fopen
        if (errno == ENOENT)
        {
            // if the file was not found, create it
            file = fopen(USERS_FILE, "a+, ccs=UTF-8");
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
                // Because the file was created, we need to set the number of users to 0
                fwprintf(file, L"%d\n", 0);
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
            freeUsers();
            fclose(file);
            return result;
        }
    }

    int number_of_users = 0;
    if (fwscanf(file, L"%d", &number_of_users) != 1)
    {
        result.Error_state = FILE_READ_ERROR;
        freeUsers();
        fclose(file);
        return result;
    }
    if (fwscanf(file, L"\n") != 0)
    {
        result.Error_state = FILE_READ_ERROR;
        freeUsers();
        return result;
    }

    if (number_of_users == 0)
    {
        User *Admin = malloc(sizeof(User));
        Admin->name = L"admin";
        Admin->pass = L"admin";
        Admin->state = ENABLED;
        Admin->type = ADMIN;
        llist_append(&users, Admin);
    }

    for (int i = 0; i < number_of_users; i++)
    {
        User *user = malloc(sizeof(User));
        if (user == 0)
        {
            result.Error_state = MALLOC_FAULT;
            freeUsers();
            fclose(file);
            return result;
        }

        user->name = malloc(sizeof(wchar_t) * USERNAME_MAX_LENGTH + 1);
        user->pass = malloc(sizeof(wchar_t) * PASSWORD_MAX_LENGTH + 1);

        if (fwscanf(file, L"%ls %ls %d %d", user->name, user->pass, &user->state, &user->type) != 4)
        {
            result.Error_state = FILE_READ_ERROR;
            freeUsers();
            fclose(file);
            return result;
        }

        // if the user isn't an admin, we read all the queued rouutes
        if (user->type == PASSANGER)
        {
            int number_of_routes = 0;
            if (fwscanf(file, L" %d", &number_of_routes) != 1)
            {
                result.Error_state = FILE_READ_ERROR;
                freeUsers();
                fclose(file);
                return result;
            }

            for (int j = 0; j < number_of_routes; j++)
            {
                // Read route id
                Route *route = calloc(1, sizeof(Route));
                if (route == 0)
                {
                    result.Error_state = MALLOC_FAULT;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                route->name = malloc(sizeof(wchar_t) * ROUTE_NAME_MAX_LENGTH + 1);
                route->destination = malloc(sizeof(wchar_t) * ROUTE_DESCRIPTION_MAX_LENGTH + 1);

                if (fwscanf(file, L" %ls", route->name) != 1)
                {
                    result.Error_state = FILE_READ_ERROR;
                    freeUsers();
                    fclose(file);
                    return result;
                }
                if (fwscanf(file, L" %ls", route->destination) != 1)
                {
                    result.Error_state = FILE_READ_ERROR;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                if (fwscanf(file, L" %d", route->state) != 1)
                {
                    result.Error_state = FILE_READ_ERROR;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                Time *tmp = calloc(1, sizeof(Time));
                if (fwscanf(file, L" %d", tmp->day) != 1)
                {
                    result.Error_state = FILE_READ_ERROR;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                if (fwscanf(file, L" %d:%d", tmp->hour, tmp->minute != 1))
                {
                    result.Error_state = FILE_READ_ERROR;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                llist_append(&route->scheduled_times, tmp);
                llist_append(&user->queued_routes, route);
            }
        }

        llist_append(&users, user);

        if (fwscanf(file, L"\n") != 0)
        {
            result.Error_state = FILE_READ_ERROR;
            freeUsers();
            fclose(file);
            return result;
        }
    }

    fclose(file);
    return result;
}

Result writeAllUsers()
{
    Result result = {OK, NULL};
    FILE *file = fopen(USERS_FILE, "w, ccs=UTF-8");
    int number_of_users = llist_size(&users);
    fwprintf(file, L"%d\n", number_of_users);
    for (int i = 0; i < number_of_users; i++)
    {
        User *user = llist_get(&users, i);
        fwprintf(file, L"%ls %ls %d %d", user->name, user->pass, user->state, user->type);
        if (user->type == PASSANGER)
        {
            int number_of_routes = llist_size(&user->queued_routes);
            fwprintf(file, L" %d", number_of_routes);
            for (int j = 0; j < number_of_routes; j++)
            {
                Route *route = llist_get(&user->queued_routes, j);
                fwprintf(file, L" %ls %ls %d", route->name, route->destination, route->state);
                Time *tmp = llist_get(&route->scheduled_times, 0);

                fwprintf(file, L" %d %d:%d", tmp->day, tmp->hour, tmp->minute);
            }
        }
        fwprintf(file, L"\n");
    }
    fclose(file);
    return result;
}

Result remove_user(const User Requester, const wchar_t *UserName)
{
    Result result = {OK, NULL};

    if (Requester.type != ADMIN)
    {
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for (int i = 0; i < llist_size(&users); i++)
    {
        User *user = llist_get(&users, i);
        if (wcscmp(user->name, UserName) == 0)
        {

            user->state = DISABLED;

            result.Error_state = OK;
            return result;
        }
    }

    result.Error_state = USER_NOT_FOUND;
    writeAllUsers();
    return result;
}

Result add_user(const User Requester, const wchar_t *NewUserName, const wchar_t *NewUserPass, const Type NewUserType)
{
    Result result = {OK, NULL};

    if (Requester.type != ADMIN)
    {
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for (int i = 0; i < llist_size(&users); i++)
    {
        User *user = llist_get(&users, i);
        if (wcscmp(user->name, NewUserName) == 0)
        {
            result.Error_state = USER_ALREADY_EXISTS;
            return result;
        }
    }

    User *user = malloc(sizeof(User));
    if (user == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    user->name = malloc(sizeof(wchar_t) * (wcslen(NewUserName) + 1));
    if (user->name == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }
    wcscpy(user->name, NewUserName);

    user->pass = malloc(sizeof(wchar_t) * (wcslen(NewUserPass) + 1));
    if (user->pass == 0)
    {
        result.Error_state = MALLOC_FAULT;
        return result;
    }
    wcscpy(user->pass, NewUserPass);

    user->state = ENABLED;
    user->type = NewUserType;

    llist_append(&users, user);
    writeAllUsers();

    result.Error_state = OK;
    return result;
}

Result modify_user( User Requester, wchar_t *UserName,  wchar_t *NewUserName,  wchar_t *NewUserPass, Type NewUserType)
{
    Result result = {OK, NULL};

    if (Requester.type != ADMIN)
    {
        result.Error_state = USER_NOT_ALLOWED;
        extern struct _Route Route;
        return result;
    }

    for (int i = 0; i < llist_size(&users); i++)
    {
        User *user = llist_get(&users, i);
        if (wcscmp(user->name, UserName) == 0)
        {
            if(NewUserName != NULL){
                free(user->name);
                user->name = malloc(sizeof(wchar_t) * (wcslen(NewUserName) + 1));
                if (user->name == 0)
                {
                    result.Error_state = MALLOC_FAULT;
                    return result;
                }
                wcscpy(user->name, NewUserName);
            }

            if(NewUserPass != NULL){
                free(user->pass);
                user->pass = malloc(sizeof(wchar_t) * (wcslen(NewUserPass) + 1));
                if (user->pass == 0)
                {
                    result.Error_state = MALLOC_FAULT;
                    return result;
                }
                wcscpy(user->pass, NewUserPass);
            }

            user->type = NewUserType;

            if (user->type == ADMIN)
            {
                User *us = query_user(Requester, UserName).Result;
                freeUserRoutes(*us);
            }

            result.Error_state = OK;
            return result;
        }
    }

    writeAllUsers();
    result.Error_state = USER_NOT_FOUND;
    return result;
}